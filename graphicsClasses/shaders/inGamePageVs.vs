layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 3) in vec2 vs_vertexUVs;
layout(location = 4) in float vs_labelId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;
out float fs_needsDiscard;

uniform float leftDigit;
uniform float middleDigit;
uniform float rightDigit;

float needsDiscard() {
    if(vs_labelId == leftDigit && leftDigit != 0.0
       || vs_labelId == middleDigit && (middleDigit != 10.0 || leftDigit != 0.0)
       || vs_labelId == rightDigit) {
        return -1.0;
    }
    return 1.0;
}

void main() {
    fs_vertexUVs = vs_vertexUVs;
    fs_vertexColor = vs_vertexColor;
    fs_isLetter = vs_labelId < 0.0
        ? -1.0
        : 1.0;

    fs_needsDiscard = needsDiscard();
    gl_Position = vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
