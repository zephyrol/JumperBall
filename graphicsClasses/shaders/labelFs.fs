uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;

in float fs_isLetter;

#ifdef(DISCARDING)
    in float fs_needsDiscard;
#endif

#ifdef(TRANSPARENT_BACKGROUND)
    in float fs_needsTransparentBackground;
#endif

out vec4 pixelColor;

void main() {
    #ifdef(DISCARDING)
        if(fs_needsDiscard > 0.0) {
            discard;
        }
    #endif
    float alpha = fs_isLetter < 0.0
        ? 1.0
        : texture(characterTexture, fs_vertexUVs).x;

    vec3 color = fs_vertexColor;
    #ifdef(TRANSPARENT_BACKGROUND)
        if(fs_needsTransparentBackground > 0.0) {
            float distUv = length(fs_vertexUVs - vec2(0.5));
            alpha = min(1.0 - (distUv * 2.0), 0.4);
            // alpha = 0.9;
            color = vec3(0.0);
        }
    #endif
    pixelColor = vec4(color, alpha);
}
