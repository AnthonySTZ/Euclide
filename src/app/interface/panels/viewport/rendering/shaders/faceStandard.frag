#version 330 core

in vec3 vertexNormal;
in vec3 vertexColor;

out vec4 outColor;

uniform vec3 lightDir = vec3(0.8f, 1.0f, 0.5f);

const float minIntensity = 0.7;

void main() {
    vec3 norm = normalize(vertexNormal);
    vec3 light = normalize(lightDir);

    float intensity = (dot(light, norm) + 1) * 0.5;              // [0, 1]
    intensity = intensity * (1.0 - minIntensity) + minIntensity; // [minInt, 1]
    vec3 diffuse = vertexColor * intensity * 0.8;

    outColor = vec4(diffuse, 1.0);
}