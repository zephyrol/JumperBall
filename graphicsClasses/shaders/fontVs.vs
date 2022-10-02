uniform float positionY;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 3) in vec2 vs_vertexUVs;
layout(location = 4) in vec2 vs_labelType;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out vec2 fs_labelType;

void main() {
    fs_vertexUVs = vs_vertexUVs;
    fs_vertexColor = vs_vertexColor;
    fs_labelType = vs_labelType;

    gl_Position = vec4(vs_vertexPosition.xy + vec2(0.0, positionY), 0.0, 1.0);
}
