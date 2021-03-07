#version 330 core

out vec4 pixelColor;
in vec3 fs_vertexColor;
in vec3 fs_vertexNormal;

void main() {
    float opacity = 1.f;
    pixelColor = vec4(fs_vertexColor, opacity);
}
