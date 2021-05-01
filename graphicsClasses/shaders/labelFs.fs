#version 330 core

in vec3 fs_vertexColor;

out vec4 pixelColor;

void main() {
    pixelColor = vec4(fs_vertexColor, 1.0);
}
