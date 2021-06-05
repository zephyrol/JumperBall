#version 330 core

uniform mat4 VP;
uniform mat4 VPStar;

uniform vec3 scale;
uniform vec3 translation;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in vec3 vs_blockPosition;

out vec3 fs_vertexColor;
out vec4 fs_vertexDepthMapSpace;
out vec3 fs_vertexNormal;
out vec3 fs_vertexPositionWorld;

void main() {
    vec3 position = (vs_vertexPosition - vs_blockPosition) * scale + vs_blockPosition + translation;
    vec4 positionVec4 = vec4(position, 1.0);

    fs_vertexColor              = vs_vertexColor;
    fs_vertexNormal             = vs_vertexNormal; // normalize((N * vec4(vs_vertexNormal,1.f)).xyz);
    fs_vertexPositionWorld      = position; 
    fs_vertexDepthMapSpace      = VPStar * positionVec4;

    gl_Position                 = VP * positionVec4;
}
