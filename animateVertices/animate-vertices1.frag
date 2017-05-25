#version 330 core

uniform sampler2D colorMap;

in vec4 frontColor;
out vec4 fragColor;

void main() {
    fragColor = frontColor;
}
