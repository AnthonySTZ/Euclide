[[vk::binding(0, 0)]] RWStructuredBuffer<float> InBufferX;
[[vk::binding(1, 0)]] RWStructuredBuffer<float> InBufferY;
[[vk::binding(2, 0)]] RWStructuredBuffer<float> InBufferZ;

[[vk::binding(3, 0)]] RWStructuredBuffer<float> OutBuffer;

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    OutBuffer[DTid.x] = 1.0;
}