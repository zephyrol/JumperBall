#version 330 core

uniform sampler2D frameTexture;
uniform float     threshold;

in      vec2      fs_vertexUVs;
out     vec4      pixelColor;


void brightPassFilter() {
  vec4 color = texture(frameTexture,fs_vertexUVs);
  float sum = color.x + color.y + color.z;
  if (sum > threshold) {
    pixelColor = color;
  } else {
    pixelColor = vec4(0.f,0.f,0.f,1.f);
  }
}

void main() {
  brightPassFilter();
}
