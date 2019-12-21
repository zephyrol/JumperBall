#version 330 core

out vec4 pixelColor;
in vec3 fs_vertexColor;
in vec3 fs_vertexNormal;

void main() {
    pixelColor = vec4(1.f,1.f,1.f,1.f);
}