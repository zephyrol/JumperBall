#version 330 core

uniform mat4 VPStar;

layout(location = 0) in vec3 vs_vertexPosition;

void main() {
    gl_Position = VPStar * vec4(vs_vertexPosition, 1.0);
}
