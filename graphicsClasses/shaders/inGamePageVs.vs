layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsCheckingKey;
out vec3 fs_keyColor;
out float fs_needsCheckingCoin;

uniform int leftDigit;
uniform int middleDigit;
uniform int rightDigit;
uniform int coinsTensDigit;
uniform int coinsUnitsDigit;
uniform int selectedLabel;
uniform int key;
uniform int currentNumberOfKeys;
uniform int maxNumberOfKeys;

bool needsDiscard() {
    if(vs_labelId == leftDigit && leftDigit != 0
       || vs_labelId == middleDigit && (middleDigit != 10 || leftDigit != 0)
       || vs_labelId == rightDigit) {
        return false;
    }
    if(vs_labelId == coinsTensDigit || vs_labelId == coinsUnitsDigit) {
        return false;
    }
    return true;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    if(selectedLabel < 0 && selectedLabel == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vec3(0.0, 1.0, 1.0);
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
        if(keyNumber > maxNumberOfKeys) {
            discarding = true;
        }
        if(keyNumber > currentNumberOfKeys) {
            fs_keyColor = vec3(0.0);
        } else {
            fs_keyColor = vec3(1.0, 1.0, 0.0);
        }
    } else {
        fs_needsCheckingKey = -1.0;
        fs_keyColor = vec3(0.0);
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
