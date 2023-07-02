layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;
layout(location = 4) in int vs_groupId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsTransparentBackground;

uniform float positionY[idCount];
uniform int levelProgression[idCount];
uniform int selectedLabel[idCount];

void main() {
    fs_vertexUVs = vs_vertexUVs;

    if(vs_labelId == 1000) {
        fs_isLetter = -1.0;
        fs_needsTransparentBackground = 1.0;
        fs_vertexColor = vs_vertexColor;
    } else {

        if(vs_labelId > levelProgression[vs_groupId]) {
            fs_vertexColor = vec3(0.0, 0.2, 0.2);
        } else if (vs_labelId == selectedLabel[vs_groupId]) {
            fs_vertexColor = vec3(1.0);
        } else {
            fs_vertexColor = vec3(0.0, 1.0, 1.0);
        }

        fs_isLetter = 1.0;
        fs_isLetter = vs_labelId == -1
            ? -1.0
            : 1.0;
        fs_needsTransparentBackground = -1.0;
    }

    vec2 positionXY = vs_vertexPosition.xy;
    bool discarding = false;
    if(vs_labelId > 0 && vs_labelId < 1000) {
        positionXY +=  vec2(0.0, positionY[vs_groupId]);
        if(positionXY.y > heightThreshold) {
            discarding = true;
        }
    }

    gl_Position = discarding ? vec4(-2.0, -2.0, 0.0, 1.0): vec4(positionXY, 0.0, 1.0);
}
