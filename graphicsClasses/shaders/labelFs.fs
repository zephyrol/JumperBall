#version 300 es

uniform Menu {
    float positionY;
};

in vec3 fs_vertexColor;

out vec4 pixelColor;

void main() {
    pixelColor = vec4(fs_vertexColor, 1.0);
}
