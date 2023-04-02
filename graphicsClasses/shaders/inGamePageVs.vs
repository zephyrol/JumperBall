layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsDiscard;

uniform int leftDigit;
uniform int middleDigit;
uniform int rightDigit;
uniform int selectedLabel;

float needsDiscard() {
    if(vs_labelId == leftDigit && leftDigit != 0
       || vs_labelId == middleDigit && (middleDigit != 10 || leftDigit != 0)
       || vs_labelId == rightDigit) {
        return -1.0;
    }
    return 1.0;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    if(selectedLabel < 0 && selectedLabel == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vs_vertexColor;
    }
    if(vs_labelId < 0) {
        fs_isLetter = -1.0;
        fs_needsDiscard = -1.0;
    } else {
        fs_isLetter = 1.0;
        fs_needsDiscard = needsDiscard();
    }
    gl_Position = vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
