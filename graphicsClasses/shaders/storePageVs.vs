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
out float fs_ball;

uniform int skinOne[idCount];
uniform int skinTwo[idCount];
uniform int skinThree[idCount];
uniform int skinFour[idCount];
uniform int skinFive[idCount];
uniform int skinSix[idCount];
uniform int sumDigitOne[idCount];
uniform int sumDigitTwo[idCount];
uniform int sumDigitThree[idCount];
uniform int sumDigitFour[idCount];
uniform int selectedLabel[idCount];

bool needsDiscard() {
    if (vs_labelId < 100) {
        return false;
    }
    int skins[6] = int[] (
        skinOne[vs_groupId],
        skinTwo[vs_groupId],
        skinThree[vs_groupId],
        skinFour[vs_groupId],
        skinFive[vs_groupId],
        skinSix[vs_groupId]
    );
    int currentSkin = 0;
    for (int i = 1000; i < 1600; i += 100) {
        if (vs_labelId < (i + 100)) {
            // Select
            if (vs_labelId == i) {
                return skins[currentSkin] == 0;
            }
            // Coins + letters
            if (vs_labelId == (i + 1) || vs_labelId == (i + 3)) {
                return skins[currentSkin] > 0;
            }
            // Ball
            if (vs_labelId == (i + 2)) {
                return false;
            }
        }
        ++currentSkin;
    }
    if (vs_labelId < 110) {
        return vs_labelId != sumDigitOne[vs_groupId];
    }
    if (vs_labelId < 120) {
        return vs_labelId != sumDigitTwo[vs_groupId];
    }
    if (vs_labelId < 130) {
        return vs_labelId != sumDigitThree[vs_groupId];
    }
    if (vs_labelId < 140) {
        return vs_labelId != sumDigitFour[vs_groupId];
    }
    return false;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    fs_vertexColor = vec3(0.0, 1.0, 1.0);

    bool discarding;

    if (vs_labelId < 0 || vs_labelId == 3) {
        fs_isLetter = -1.0;
        discarding = false;
    } else {
        fs_isLetter = 1.0;
        discarding = needsDiscard();
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

    if (vs_labelId == 1002) {
        fs_isLetter = -1.0;
        fs_ball = 1.0;
    } else if (vs_labelId == 1102) {
        fs_isLetter = -1.0;
        fs_ball = 2.0;
    } else if (vs_labelId == 1202) {
        fs_isLetter = -1.0;
        fs_ball = 3.0;
    } else if (vs_labelId == 1302) {
        fs_isLetter = -1.0;
        fs_ball = 4.0;
    } else if (vs_labelId == 1402) {
        fs_isLetter = -1.0;
        fs_ball = 5.0;
    } else if (vs_labelId == 1502) {
        fs_isLetter = -1.0;
        fs_ball = 6.0;
    } else {
        fs_ball = -1.0;
    }

    if(selectedLabel[vs_groupId] == vs_labelId && (
        vs_labelId == 1000
        || vs_labelId == 1001
        || vs_labelId == 1100
        || vs_labelId == 1101
        || vs_labelId == 1200
        || vs_labelId == 1201
        || vs_labelId == 1300
        || vs_labelId == 1301
        || vs_labelId == 1400
        || vs_labelId == 1401
        || vs_labelId == 1501
        || vs_labelId == 1502
        || vs_labelId == 4000
    )) {
        fs_vertexColor = vec3(1.0);
    }

    if (vs_labelId >= 2000 && vs_labelId != 4000) {
        fs_needsTransparentBackground = 1.0;
    } else {
        fs_needsTransparentBackground = -1.0;
    }

    int skins[6] = int[] (
        skinOne[vs_groupId],
        skinTwo[vs_groupId],
        skinThree[vs_groupId],
        skinFour[vs_groupId],
        skinFive[vs_groupId],
        skinSix[vs_groupId]
    );

    for (int i = 0; i < 6; ++i) {
        if ((i + 2000) == vs_labelId && skins[i] == 2) {
            fs_needsTransparentBackground = 20.0;
        }
    }

    if (vs_labelId == 4000) {
        fs_isLetter = -1.0;
        discarding = false;
    }

    // Coin symbol
    if(vs_labelId == 400) {
        discarding = false;
        fs_isLetter = -1.0;
        fs_needsCheckingCoin = 1.0;
    }
    gl_Position = discarding ? vec4(-2.0, -2.0, 0.0, 1.0): vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
