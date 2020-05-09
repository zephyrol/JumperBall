#version 330 core


uniform sampler2D characterTexture;
uniform vec3      fontColor;

in      vec2      fs_vertexUVs;
out     vec4      pixelColor;


void main() {
    vec4 sampledColor = texture(characterTexture,fs_vertexUVs);

    //pixelColor = vec4(fontColor,sampledColor.x);
    pixelColor = vec4(0,0,1,1);
}
