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

uniform int skinOne;
uniform int skinTwo;
uniform int skinThree;
uniform int skinFour;
uniform int skinFive;
uniform int skinSix;
uniform int selectedLabel;

float needsDiscard() {
    if (vs_labelId < 100) {
        return -1.0;
    }
    int skins[6] = int[] (
    skinOne, skinTwo, skinThree, skinFour, skinFive, skinSix
    );
    int currentSkin = 0;
    for (int i = 1000; i < 1600; i += 100) {
        if (vs_labelId < (i + 100)) {
            // Select
            if (vs_labelId == i) {
                return skins[currentSkin] == 0 ? 1.0 : -1.0;
            }
            // Coins + letters
            if (vs_labelId == (i + 1) || vs_labelId == (i + 3)) {
                return skins[currentSkin] > 0 ? 1.0 : -1.0;
            }
            // Ball
            if (vs_labelId == (i + 2)) {
                return -1.0;
            }
        }
        ++currentSkin;
    }
    return -1.0;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    if (selectedLabel < 0 && selectedLabel == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vec3(0.0, 1.0, 1.0);
    }
    if (vs_labelId < 0 || vs_labelId == 3) {
        fs_isLetter = -1.0;
        fs_needsDiscard = -1.0;
    } else {
        fs_isLetter = 1.0;
        fs_needsDiscard = needsDiscard();
    }

    // Coin symbol
    if (
    vs_labelId == 1003
    || vs_labelId == 1103
    || vs_labelId == 1203
    || vs_labelId == 1303
    || vs_labelId == 1403
    || vs_labelId == 1503
    ) {
        fs_isLetter = -1.0;
        fs_needsCheckingCoin = 1.0;
    } else {
        fs_needsCheckingCoin = -1.0;
    }

    if (vs_labelId == 2000) {
        fs_needsTransparentBackground = 1.0;
    } else {
        fs_needsTransparentBackground = -1.0;
    }
    gl_Position = vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
