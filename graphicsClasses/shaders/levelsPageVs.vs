layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec2 vs_vertexUVs;
layout(location = 3) in int vs_labelId;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out float fs_isLetter;

uniform float positionY;
uniform int levelProgression;

void main() {
    fs_vertexUVs = vs_vertexUVs;
    fs_vertexColor = vs_labelId < 1 || vs_labelId <= levelProgression
        ? vs_vertexColor
        : vs_vertexColor * 0.2;

    fs_isLetter = vs_labelId < 0
        ? -1.0
        : 1.0;

    vec2 positionXY = vs_vertexPosition.xy + vec2(0.0, positionY);
    gl_Position = vec4(positionXY, 0.0, 1.0);
}
