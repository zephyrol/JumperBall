uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;
in vec2 fs_labelType;

out vec4 pixelColor;

void main() {

    float alpha = fs_labelType.x == 0.f
        ? 1.0
        : texture(characterTexture, vec2(fs_vertexUVs.x, fs_vertexUVs.y)).x;

    pixelColor = vec4(fs_vertexColor, alpha);
}
