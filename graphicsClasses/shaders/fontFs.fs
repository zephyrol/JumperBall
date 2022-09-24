uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;

out vec4 pixelColor;

void main() {
    if(fs_vertexUVs == vec2(0.0, 0.0)) {
        pixelColor = vec4(1.f, 1.f, 0.f, 1.f);
    } else {
        vec4 sampledColor = texture(characterTexture, vec2(fs_vertexUVs.x, fs_vertexUVs.y));
        pixelColor = vec4(fs_vertexColor, sampledColor.x);
        pixelColor = vec4(1.f, 1.f, 0.f, 1.f);
    }
}
