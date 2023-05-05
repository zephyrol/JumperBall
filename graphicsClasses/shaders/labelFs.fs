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

#ifdef(TEST_KEY)
    in float fs_needsCheckingKey;
    in vec3 fs_keyColor;
#endif

#ifdef(TEST_COIN)
    in float fs_needsCheckingCoin;
#endif

out vec4 pixelColor;

#ifdef(TEST_KEY)
    bool isInKey() {
        float x = fs_vertexUVs.x;
        float y = fs_vertexUVs.y;
        const float rightKeyBody = 0.55;
        const float rightKeyEnd = 0.75;
        if(x > 0.45 && x < rightKeyBody && y < 0.8) {
            return true;
        }
        if(x > rightKeyBody && x < rightKeyEnd ) {
            if(y < 0.1) {
                return true;
            }
            if(y > 0.2 && y < 0.3) {
                return true;
            }
        }
        const float headRadius = 0.2;
        const vec2 headCenter = vec2(0.5, 1.0 - headRadius);
        vec2 centerToUv = fs_vertexUVs - headCenter;
        if(length(centerToUv) < headRadius) {
            return true;
        }
        return false;
    }
#endif

#ifdef(TEST_COIN)
vec3 getCoinColor() {
    float x = fs_vertexUVs.x - 0.5;
    float y = fs_vertexUVs.y - 0.5;
    float pixelDistance = sqrt(x * x + y * y);
    if(pixelDistance > 0.5) {
        discard;
    }
    return mix(vec3(0.95, 0.95, 0.8), vec3(0.5, 0.25, 0.0), pixelDistance * 2.0);
}
#endif

void main() {
    #ifdef(DISCARDING)
        if(fs_needsDiscard > 0.0) {
            discard;
        }
    #endif

    vec3 color = fs_vertexColor;
    #ifdef(TEST_KEY)
        if(fs_needsCheckingKey > 0.0) {
            if (!isInKey()) {
                discard;
            }
            color = fs_keyColor;
        }
    #endif

    #ifdef(TEST_COIN)
    if(fs_needsCheckingCoin > 0.0) {
        color = getCoinColor();
    }
    #endif

    float alpha = fs_isLetter < 0.0
        ? 1.0
        : texture(characterTexture, fs_vertexUVs).x;

    #ifdef(TRANSPARENT_BACKGROUND)
        if(fs_needsTransparentBackground > 0.0) {
            float distUv = length(fs_vertexUVs - vec2(0.5));
            alpha = min(1.0 - (distUv * 2.0), 0.4);
            color = vec3(0.0);
        }
    #endif
    pixelColor = vec4(color, alpha);
}
