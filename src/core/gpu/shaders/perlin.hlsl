[[vk::binding(0, 0)]] RWStructuredBuffer<float> InBufferX;
[[vk::binding(1, 0)]] RWStructuredBuffer<float> InBufferY;
[[vk::binding(2, 0)]] RWStructuredBuffer<float> InBufferZ;

[[vk::binding(3, 0)]] RWStructuredBuffer<float> OutBuffer;

[[vk::binding(4, 0)]] RWStructuredBuffer<int> p;

[[vk::binding(5, 0)]] cbuffer ParamsBuffer
{
    float3 frequency;
    float pad_;
    int octaves;
    int numPoints;
};

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float a, float b, float x) {
    return a + x * (b - a);
}

float grad(int hash, float x, float y, float z) {
    int h = hash & 15;									// Take the hashed value and take the first 4 bits of it (15 == 0b1111)
    float u = h < 8 /* 0b1000 */ ? x : y;				// If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.
    
    float v;											// In Ken Perlin's original implementation this was another conditional operator (?:).  I
                                                        // expanded it for readability.
    
    if(h < 4 /* 0b0100 */)								// If the first and second significant bits are 0 set v = y
        v = y;
    else if(h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)// If the first and second significant bits are 1 set v = x
        v = x;
    else 												// If the first and second significant bits are not equal (0/1, 1/0) set v = z
        v = z;
    
    return ((h&1) == 0 ? u : -u)+((h&2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
}

float perlin(float x, float y, float z) {
    int xi = int(floor(x)) & 255;
    int yi = int(floor(y)) & 255;
    int zi = int(floor(z)) & 255;

    int inc_xi = ((xi + 1) & 255);
    int inc_yi = ((yi + 1) & 255);
    int inc_zi = ((zi + 1) & 255);

    int corner_000 = p[p[p[  xi  ]+  yi  ]+  zi  ];
    int corner_010 = p[p[p[  xi  ]+inc_yi]+  zi  ];
    int corner_001 = p[p[p[  xi  ]+  yi  ]+inc_zi];
    int corner_011 = p[p[p[  xi  ]+inc_yi]+inc_zi];
    int corner_100 = p[p[p[inc_xi]+  yi  ]+  zi  ];
    int corner_110 = p[p[p[inc_xi]+inc_yi]+  zi  ];
    int corner_101 = p[p[p[inc_xi]+  yi  ]+inc_zi];
    int corner_111 = p[p[p[inc_xi]+inc_yi]+inc_zi];

    float xf = x-int(floor(x));
    float yf = y-int(floor(y));
    float zf = z-int(floor(z));
    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    float x1 = lerp(grad(corner_000, xf, yf, zf), grad(corner_100, xf-1, yf, zf), u);								
    float x2 = lerp(grad(corner_010, xf, yf-1, zf), grad(corner_110, xf-1, yf-1, zf), u);
    float y1 = lerp(x1, x2, v);

    x1 = lerp(grad(corner_001, xf, yf, zf-1), grad(corner_101, xf-1, yf, zf-1), u);
    x2 = lerp(grad(corner_011, xf, yf-1, zf-1), grad(corner_111, xf-1, yf-1, zf-1), u);
    float y2 = lerp(x1, x2, v);

    return (lerp(y1, y2, w) + 1) * 0.5;
}

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= numPoints) return;

    float x = InBufferX[DTid.x];
    float y = InBufferY[DTid.x];
    float z = InBufferZ[DTid.x];

    float total = 0;
    float3 acc_frequency = frequency;
    float acc_amplitude = 1;
    float persistence = 1;
    float maxValue = 0;			// Used for normalizing result to 0.0 - 1.0
    for(int i = 0; i < octaves; i++) {
        total += perlin(x * acc_frequency.x, y * acc_frequency.y, z * acc_frequency.z) * acc_amplitude;
        
        maxValue += acc_amplitude;
        
        acc_amplitude *= persistence;
        acc_frequency *= 2;
    }
    
    OutBuffer[DTid.x] = total/maxValue;
}