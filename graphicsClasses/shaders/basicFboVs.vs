#version 330 core



layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 3) in vec2 vs_vertexUVs;

out vec2 fs_vertexUVs;

void main() {
    const float w               = 1.f;

    fs_vertexUVs                = vs_vertexUVs;
    gl_Position                 = vec4(vs_vertexPosition, w);
}
