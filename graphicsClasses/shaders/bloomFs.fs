#version 330 core


uniform sampler2D frameToneMappingTexture;
uniform sampler2D frameBrightPassFilterTexture;

in      vec2      fs_vertexUVs;
out     vec4      pixelColor;

void main() {
    pixelColor = texture(frameToneMappingTexture, fs_vertexUVs) +
                  texture(frameBrightPassFilterTexture, fs_vertexUVs);
}

