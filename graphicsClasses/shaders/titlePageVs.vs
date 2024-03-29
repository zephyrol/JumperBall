layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;
layout(location = 4) in int vs_groupId;

out vec3 fs_vertexColor;
out vec2 fs_vertexUVs;

out float fs_isLetter;
out float fs_needsTransparentBackground;

uniform int selectedLabel[idCount];

void main() {
    fs_vertexUVs = vs_vertexUVs;
    int selectedLabelValue = selectedLabel[vs_groupId];
    if(selectedLabelValue > 0 && selectedLabelValue == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vec3(0.0, 1.0, 1.0);
    }
    if(vs_labelId == 1000) {
        fs_isLetter = -1.0;
        fs_needsTransparentBackground = 1.0;
    } else {
        fs_isLetter = 1.0;
        fs_needsTransparentBackground = -1.0;
    }
    gl_Position = vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
