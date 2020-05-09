#version 330 core


uniform sampler2D characterTexture;
uniform vec3      fontColor;

in      vec2      fs_vertexUVs;
out     vec4      pixelColor;


void main() {
    vec4 sampledColor = texture(characterTexture,vec2(fs_vertexUVs.x,
                                                      1.f-fs_vertexUVs.y));

    pixelColor = vec4(fontColor,sampledColor.x);
    //pixelColor = vec4(1.f,1.f,0,1);
}
