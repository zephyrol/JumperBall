layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;
layout(location = 4) in int vs_groupId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsCheckingKey;
out vec3 fs_keyColor;
out float fs_needsCheckingCoin;

uniform float missingTimeWarning[idCount];
uniform int leftDigit[idCount];
uniform int middleDigit[idCount];
uniform int rightDigit[idCount];
uniform int coinsTensDigit[idCount];
uniform int coinsUnitsDigit[idCount];
uniform int selectedLabel[idCount];
uniform int currentNumberOfKeys[idCount];
uniform int maxNumberOfKeys[idCount];

bool needsDiscard() {
    int leftDigitValue = leftDigit[vs_groupId];
    int middleDigitValue = middleDigit[vs_groupId];
    int rightDigitValue = rightDigit[vs_groupId];
    if(vs_labelId == leftDigitValue && leftDigitValue != 0
       || vs_labelId == middleDigitValue && (middleDigitValue != 10 || leftDigitValue != 0)
       || vs_labelId == rightDigitValue) {
        return false;
    }
    if(vs_labelId == coinsTensDigit[vs_groupId] || vs_labelId == coinsUnitsDigit[vs_groupId]) {
        return false;
    }
    return true;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    int selectedLabelValue = selectedLabel[vs_groupId];
    if(selectedLabelValue < 0 && selectedLabelValue == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vec3(0.0, 1.0, 1.0);
    }

    if(vs_labelId == rightDigit[vs_groupId]) {
        fs_vertexColor = mix(fs_vertexColor, vec3(1.0, 0.0, 0.0), missingTimeWarning[vs_groupId]);
    }

    bool discarding;
    if(vs_labelId < 0) {
        fs_isLetter = -1.0;
        discarding = false;
    } else {
        fs_isLetter = 1.0;
        discarding = needsDiscard();
    }

    const int keyIdOffset = 500;
    if(vs_labelId > keyIdOffset) {
        fs_isLetter = -1.0;
        int keyNumber = vs_labelId - keyIdOffset;
        fs_needsCheckingKey = 1.0;
        discarding = false;
        if(keyNumber > maxNumberOfKeys[vs_groupId]) {
            discarding = true;
        }
        if(keyNumber > currentNumberOfKeys[vs_groupId]) {
            fs_keyColor = vec3(0.3);
        } else {
            fs_keyColor = vec3(1.0, 1.0, 0.0);
        }
    } else {
        fs_needsCheckingKey = -1.0;
        fs_keyColor = vec3(0.0);

        if(vs_labelId >= 401) {
            fs_isLetter = -1.0;
            discarding = false;
        }

    }

    // Coin symbol
    if(vs_labelId == 400) {
        discarding = false;
        fs_isLetter = -1.0;
        fs_needsCheckingCoin = 1.0;
    } else {
        fs_needsCheckingCoin = -1.0;
    }

    gl_Position = discarding ? vec4(-2.0, -2.0, 0.0, 1.0): vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
