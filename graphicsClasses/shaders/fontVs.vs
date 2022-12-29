layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 3) in vec2 vs_vertexUVs;
layout(location = 4) in vec2 vs_labelType;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out vec2 fs_labelType;

#ifdef(TIMER)
    out float fs_needsDiscard;
#endif

#ifdef(SCROLLABLE)
    uniform float positionY;
#endif

#ifdef(TIMER)
    uniform float leftDigit;
    uniform float middleDigit;
    uniform float rightDigit;
#endif

#ifdef(TIMER)
    float needsDiscard() {

        if(fs_labelType.x == 2.0) {
            if (leftDigit != 0 && fs_labelType.y == leftDigit) {
                return 0.0;
            }
            return 1.0;
        }

        if(fs_labelType.x == 3.0) {
            if (fs_labelType.y == middleDigit && (leftDigit != 0 || middleDigit != 0)) {
                return 0.0;
            }
            return 1.0;
        }
        if(fs_labelType.x == 4.0) {
            if (fs_labelType.y == rightDigit) {
                return 0.0;
            }
            return 1.0;
        }
        return 0.0;
    }

#endif

void main() {
    fs_vertexUVs = vs_vertexUVs;
    fs_vertexColor = vs_vertexColor;
    fs_labelType = vs_labelType;

    #ifdef(TIMER)
        fs_needsDiscard = needsDiscard();
    #endif

    vec2 positionXY = vs_vertexPosition.xy;
    #ifdef(SCROLLABLE)
        positionXY += vec2(0.0, positionY);
    #endif
    gl_Position = vec4(positionXY, 0.0, 1.0);
}
