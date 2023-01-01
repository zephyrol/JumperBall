uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;

#ifdef(TEST_ALPHA_TEXTURE)
    in float fs_isLetter;
#endif

#ifdef(DISCARDING)
    in float fs_needsDiscard;
#endif

out vec4 pixelColor;

void main() {
    #ifdef(DISCARDING)
        if(fs_needsDiscard > 0.0) {
            discard;
        }
    #endif
    #ifdef(TEST_ALPHA_TEXTURE)
        float alpha = fs_isLetter < 0.0
            ? 1.0
            : texture(characterTexture, fs_vertexUVs).x;
    #endif
    #ifdef(ALWAYS_ALPHA_TEXTURE)
        float alpha = texture(characterTexture, fs_vertexUVs).x;
    #endif
    vec3 color = fs_vertexColor;
    pixelColor = vec4(color, alpha);
}
