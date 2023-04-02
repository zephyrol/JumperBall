layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;

out vec3 fs_vertexColor;
out vec2 fs_vertexUVs;

out float fs_isLetter;
out float fs_needsTransparentBackground;

uniform int selectedLabel;

void main() {
    fs_vertexUVs = vs_vertexUVs;
    if(selectedLabel > 0 && selectedLabel == vs_labelId) {
        fs_vertexColor = vec3(1.0);
    } else {
        fs_vertexColor = vec3(0.0, 1.0, 1.0);
    }
    if(vs_labelId == 1000) {
        fs_isLetter = -1.f;
        fs_needsTransparentBackground = 1.f;
    } else {
        fs_isLetter = 1.f;
        fs_needsTransparentBackground = -1.f;
    }
    gl_Position = vec4(vs_vertexPosition.xy, 0.0, 1.0);
}
