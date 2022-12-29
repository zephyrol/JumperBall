uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;
in vec2 fs_labelType;

#ifdef(TIMER)
    in float fs_needsDiscard;
#endif

out vec4 pixelColor;

#ifdef(SCROLLABLE)
    uniform float levelProgression;
#endif

void main() {
    float alpha = fs_labelType.x < 1.0
        ? 1.0
        : texture(characterTexture, vec2(fs_vertexUVs.x, fs_vertexUVs.y)).x;

    #ifdef(TIMER)
        if(fs_needsDiscard == 1.0) {
            discard;
        }
    #endif

    vec3 color = fs_vertexColor;
    #ifdef(SCROLLABLE)
        color = fs_labelType.x == 1.0 && fs_labelType.y > 0.0 && fs_labelType.y > levelProgression
            ? fs_vertexColor * 0.2
            : fs_vertexColor;
    #endif
    pixelColor = vec4(color, alpha);
}
