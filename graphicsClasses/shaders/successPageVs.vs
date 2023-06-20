layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
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

bool needsDiscard() {
    if(vs_labelId < 100) {
        return false;
    }
    if(vs_labelId < 110) {
        return vs_labelId != previousDigitOne;
    }
    if(vs_labelId < 120) {
        return vs_labelId != previousDigitTwo;
    }
    if(vs_labelId < 130) {
        return vs_labelId != previousDigitThree;
    }
    if(vs_labelId < 140) {
        return vs_labelId != previousDigitFour;
    }
    if(vs_labelId < 150) {
        return vs_labelId != obtainedDigitOne;
    }
    if(vs_labelId < 160) {
        return vs_labelId != obtainedDigitTwo;
    }
    if(vs_labelId < 170) {
        return vs_labelId != sumDigitOne;
    }
    if(vs_labelId < 180) {
        return vs_labelId != sumDigitTwo;
    }
    if(vs_labelId < 190) {
        return vs_labelId != sumDigitThree;
    }
    if(vs_labelId < 200) {
        return vs_labelId != sumDigitFour;
    }
    return false;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    if(selectedLabel > 0 && selectedLabel < 3 && selectedLabel == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vec3(0.0, 1.0, 1.0);
    }
    bool discarding;
    if(vs_labelId < 0 || vs_labelId == 3) {
        fs_isLetter = -1.0;
        discarding = false;
    } else {
        fs_isLetter = 1.0;
        discarding = needsDiscard();
    }

    // Coin symbol
    if(vs_labelId == 400) {
        discarding = false;
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

    gl_Position = discarding ? vec4(-2.0, -2.0, 0.0, 1.0): vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
