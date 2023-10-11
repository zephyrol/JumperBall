uniform sampler2D characterTexture;

in vec3 fs_vertexColor;
in vec2 fs_vertexUVs;

in float fs_isLetter;

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

#ifdef(TEST_BALL)
    in float fs_ball;
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


#ifdef(TEST_BALL)

vec3 getBallColor() {
    float x = (fs_vertexUVs.x - 0.5) * 2.0;
    float y = (fs_vertexUVs.y - 0.5) * 2.0;
    float pixelDistance = sqrt(x * x + y * y);
    // Check https://www.canva.com/colors/color-wheel/ to choose colors
    if(pixelDistance > 1.0) {
        discard;
    }
    if (fs_ball > 5.5) {
        const float tetradricFirstThreshold = -0.5;
        const float tetradricSecondThreshold = 0.0;
        const float tetradricThirdThreshold = 0.5;
        const vec3 tetradricFirstColor = vec3(34.0 / 255.0, 221.0 / 255.0, 213.0 / 255.0);
        const vec3 tetradricSecondColor = vec3(119.0 / 255.0, 34.0 / 255.0, 221.0 / 255.0);
        if(y > tetradricThirdThreshold) {
            return tetradricFirstColor;
        }
        if(y > tetradricSecondThreshold) {
            return tetradricSecondColor;
        }
        if(y > tetradricFirstThreshold) {
            return vec3(1.0) - tetradricFirstColor;
        }
        return vec3(1.0) - tetradricSecondColor;
    }
    const float triadicFirstThreshold = -1.0 / 3.0;
    const float triadicSecondThreshold = -triadicFirstThreshold;
    if (fs_ball > 4.5) {
        if(y > triadicSecondThreshold) {
            const vec3 secondTriadicFirstColor = vec3(223.0 / 255.0, 142.0 / 255.0, 32.0 / 255.0);
            return secondTriadicFirstColor;
        }
        if(y > triadicFirstThreshold) {
            const vec3 secondTriadicSecondColor = vec3(32.0 / 255.0, 223.0 / 255.0, 142.0 / 255.0);
            return secondTriadicSecondColor;
        }
        const vec3 secondTriadicThirdColor = vec3(142.0 / 255.0, 32.0 / 255.0, 223.0 / 255.0);
        return secondTriadicThirdColor;
    }
    if (fs_ball > 3.5) {
        if(y > triadicSecondThreshold) {
            const vec3 firstTriadicFirstColor = vec3(198.0 / 255.0, 218.0 / 255.0, 37.0 / 255.0);
            return firstTriadicFirstColor;
        }
        if(y > triadicFirstThreshold) {
            const vec3 firstTriadicSecondColor = vec3(37.0 / 255.0, 198.0 / 255.0, 218.0 / 255.0);
            return firstTriadicSecondColor;
        }
        const vec3 firstTriadicThirdColor = vec3(218.0 / 255.0, 37.0 / 255.0, 198.0 / 255.0);
        return firstTriadicThirdColor;
    }
    if (fs_ball > 2.5) {
        const vec3 thirdLightColor = vec3(64.0 / 255.0, 42.0 / 255.0, 213.0 / 255.0);
        const vec3 thirdDarkColor = vec3(1.0) - thirdLightColor;
        return y > 0.0 ? thirdLightColor : thirdDarkColor;
    }
    if (fs_ball > 1.5) {
        const vec3 secondLightColor = vec3(44.0 / 255.0, 211.0 / 255.0, 208.0 / 255.0);
        const vec3 secondDarkColor = vec3(1.0) - secondLightColor;
        return y > 0.0 ? secondLightColor : secondDarkColor;
    }
    const vec3 firstLightColor = vec3(32.0 / 255.0, 223.0 / 255.0, 123.0 / 255.0);
    const vec3 firstDarkColor = vec3(1.0) - firstLightColor;
    return y > 0.0 ? firstLightColor : firstDarkColor;
}

#endif

void main() {

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

    #ifdef(TEST_BALL)
        if(fs_ball > 0.0) {
            color = getBallColor();
        }
    #endif

    float alpha = fs_isLetter < 0.0
        ? 1.0
        : texture(characterTexture, fs_vertexUVs).r;

    #ifdef(TRANSPARENT_BACKGROUND)
        if(fs_needsTransparentBackground > 0.0) {
            float distUv = length(fs_vertexUVs - vec2(0.5));
            alpha = min(1.0 - (distUv * 2.0), 0.4);
            if(fs_needsTransparentBackground > 10.0) {
                color = vec3(1.0);
                alpha *= 1.5;
            } else {
                color = vec3(0.0);
            }
        }
    #endif
    pixelColor = vec4(color, alpha);
}
