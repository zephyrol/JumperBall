layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsDiscard;

uniform float positionY;
uniform int levelProgression;
uniform int selectedLabel;

void main() {
    fs_vertexUVs = vs_vertexUVs;

    if(vs_labelId > levelProgression) {
       fs_vertexColor = vs_vertexColor * 0.2;
    } else if (vs_labelId == selectedLabel) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vs_vertexColor;
    }

    fs_isLetter = vs_labelId == -1
        ? -1.0
        : 1.0;

    vec2 positionXY = vs_vertexPosition.xy;
    fs_needsDiscard = -1.0;
    if(vs_labelId > 0) {
        positionXY +=  vec2(0.0, positionY);
        if(positionXY.y > heightThreshold) {
            fs_needsDiscard = 1.0;
        }
    }

    gl_Position = vec4(positionXY, 0.0, 1.0);
}
