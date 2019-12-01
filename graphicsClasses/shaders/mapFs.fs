#version 330 core

out vec4 pixelColor;
in vec4 fs_vertexColor;

void main() {
    const float epsilon = 0.99999f;
    if (fs_vertexColor.a < epsilon ) discard;
  pixelColor = vec4(fs_vertexColor);
}