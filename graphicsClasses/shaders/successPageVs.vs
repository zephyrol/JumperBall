layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsDiscard;
out float fs_needsCheckingCoin;
out float fs_needsTransparentBackground;

uniform int previousDigitOne;
uniform int previousDigitTwo;
uniform int previousDigitThree;
uniform int previousDigitFour;
uniform int obtainedDigitOne;
uniform int obtainedDigitTwo;
uniform int sumDigitOne;
uniform int sumDigitTwo;
uniform int sumDigitThree;
uniform int sumDigitFour;
uniform int selectedLabel;

float needsDiscard() {
    if(vs_labelId < 100) {
        return -1.0;
    }
    if(vs_labelId < 110) {
        if(vs_labelId != previousDigitOne) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 120) {
        if(vs_labelId != previousDigitTwo) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 130) {
        if(vs_labelId != previousDigitThree) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 140) {
        if(vs_labelId != previousDigitFour) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 150) {
        if(vs_labelId != obtainedDigitOne) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 160) {
        if(vs_labelId != obtainedDigitTwo) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 170) {
        if(vs_labelId != sumDigitOne) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 180) {
        if(vs_labelId != sumDigitTwo) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 190) {
        if(vs_labelId != sumDigitThree) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    if(vs_labelId < 200) {
        if(vs_labelId != sumDigitFour) {
            return 1.0;
        } else {
            return -1.0;
        }
    }
    return -1.0;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    if(selectedLabel < 0 && selectedLabel == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vec3(0.0, 1.0, 1.0);
    }
    if(vs_labelId < 0 || vs_labelId == 3) {
        fs_isLetter = -1.0;
        fs_needsDiscard = -1.0;
    } else {
        fs_isLetter = 1.0;
        fs_needsDiscard = needsDiscard();
    }

    // Coin symbol
    if(vs_labelId == 400) {
        fs_needsDiscard = -1.0;
        fs_isLetter = -1.0;
        fs_needsCheckingCoin = 1.0;
    } else {
        fs_needsCheckingCoin = -1.0;
    }

    if(vs_labelId == 1000) {
        fs_needsTransparentBackground = 1.0;
    } else {
        fs_needsTransparentBackground = -1.0;
    }

    gl_Position = vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
