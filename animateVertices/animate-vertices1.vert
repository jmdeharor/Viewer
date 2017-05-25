#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

const float PI = 3.141592654;

out vec4 frontColor;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform float amplitude=0.1;
uniform float freq = 1; // expressada en Hz
uniform float time;

void main() {
    frontColor = vec4(normalize(normalMatrix * normal).z);
    float d = amplitude*sin(2*PI*time*freq + 2*PI*texCoord.x);
    gl_Position = modelViewProjectionMatrix * vec4(vertex + d*normal, 1.0);
}
