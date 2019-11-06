#version 330 core

layout (location=0) in vec3 vs_vertexPosition;
layout (location=1) in vec3 vs_vertexColor;

out vec3 fs_vertexColor;

void main() {
  fs_vertexColor = vs_vertexColor;
  float w = 1.f;
  //gl_Position = vec4(vs_vertexPosition, w);
  gl_Position.xyz = vs_vertexPosition;
  gl_Position.w= 1.0;
}
