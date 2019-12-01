#version 330 core

out vec4 pixelColor;
in vec3 fs_vertexColor;
in float proximityObjectBehind;

void main() {
    const float epsilon = 0.999f;
    if (proximityObjectBehind < epsilon ) discard;
  pixelColor = vec4(fs_vertexColor,1.f);
}