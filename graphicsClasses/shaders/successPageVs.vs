layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;
layout(location = 4) in int vs_groupId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsCheckingCoin;
out float fs_needsTransparentBackground;

uniform int previousDigitOne[idCount];
uniform int previousDigitTwo[idCount];
uniform int previousDigitThree[idCount];
uniform int previousDigitFour[idCount];
uniform int obtainedDigitOne[idCount];
uniform int obtainedDigitTwo[idCount];
uniform int sumDigitOne[idCount];
uniform int sumDigitTwo[idCount];
uniform int sumDigitThree[idCount];
uniform int sumDigitFour[idCount];
uniform int selectedLabel[idCount];

bool needsDiscard() {
    if(vs_labelId < 100) {
        return false;
    }
    if(vs_labelId < 110) {
        return vs_labelId != previousDigitOne[vs_groupId];
    }
    if(vs_labelId < 120) {
        return vs_labelId != previousDigitTwo[vs_groupId];
    }
    if(vs_labelId < 130) {
        return vs_labelId != previousDigitThree[vs_groupId];
    }
    if(vs_labelId < 140) {
        return vs_labelId != previousDigitFour[vs_groupId];
    }
    if(vs_labelId < 150) {
        return vs_labelId != obtainedDigitOne[vs_groupId];
    }
    if(vs_labelId < 160) {
        return vs_labelId != obtainedDigitTwo[vs_groupId];
    }
    if(vs_labelId < 170) {
        return vs_labelId != sumDigitOne[vs_groupId];
    }
    if(vs_labelId < 180) {
        return vs_labelId != sumDigitTwo[vs_groupId];
    }
    if(vs_labelId < 190) {
        return vs_labelId != sumDigitThree[vs_groupId];
    }
    if(vs_labelId < 200) {
        return vs_labelId != sumDigitFour[vs_groupId];
    }
    return false;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    int selectedLabelValue = selectedLabel[vs_groupId];
    if(selectedLabelValue > 0 && selectedLabelValue < 3 && selectedLabelValue == vs_labelId) {
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
