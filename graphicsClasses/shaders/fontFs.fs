uniform Menu {
    float positionY;
};

uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;

out vec4 pixelColor;

void main() {
    vec4 sampledColor = texture(characterTexture, vec2(fs_vertexUVs.x, 1.0 - fs_vertexUVs.y));
    pixelColor = vec4(fs_vertexColor, sampledColor.x);
}
