#version 330 core


in      vec4      fs_vertexColor;
out     vec4      pixelColor;


void main() {
    pixelColor = fs_vertexColor;
}
