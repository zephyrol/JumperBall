uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;
in vec2 fs_labelType;

out vec4 pixelColor;

#ifdef(SCROLLABLE)
    uniform float levelProgression;
#endif

#ifdef(TIMER)
    uniform float leftDigit;
    uniform float middleDigit;
    uniform float rightDigit;
#endif

void main() {

    float alpha = fs_labelType.x != 1.0
        ? 1.0
        : texture(characterTexture, vec2(fs_vertexUVs.x, fs_vertexUVs.y)).x;

    vec3 color = fs_vertexColor;
    #ifdef(SCROLLABLE)
        color = fs_labelType.x == 1.0 && fs_labelType.y > 0.0 && fs_labelType.y > levelProgression
            ? fs_vertexColor * 0.2
            : fs_vertexColor;
    #endif
    pixelColor = vec4(color, alpha);
}
