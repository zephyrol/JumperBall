#version 330 core


uniform sampler2D frameTexture;
in      vec2  fs_vertexUVs;
out     vec4  pixelColor;

void main() {
    pixelColor = texture(frameTexture,fs_vertexUVs);
}
