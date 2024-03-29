layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec2 vs_vertexUVs;

out vec2 fs_vertexUVs;

void main() {
    fs_vertexUVs = vs_vertexUVs;
    gl_Position = vec4(vs_vertexPosition, 1.0);
}
