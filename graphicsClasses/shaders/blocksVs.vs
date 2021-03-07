#version 330 core

uniform mat4 VP;
uniform mat4 VPStar;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;

out vec3 fs_vertexColor;
out vec4 fs_vertexDepthMapSpace;
out vec3 fs_vertexNormal;
out vec3 fs_vertexPositionWorld;

void main() {
    const float w               = 1.f;
    vec4 vertexPositionVec4     = vec4(vs_vertexPosition, w);

    vec4 vertexPositionWorldSpace = vertexPositionVec4;

    fs_vertexColor              = vs_vertexColor;
    fs_vertexNormal             = vs_vertexNormal; // normalize((N * vec4(vs_vertexNormal,1.f)).xyz);
    fs_vertexPositionWorld      = vertexPositionWorldSpace.xyz;
    fs_vertexDepthMapSpace      = VPStar * vertexPositionWorldSpace;

    gl_Position                 = VP * vertexPositionWorldSpace;
}
