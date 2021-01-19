#version 330 core

uniform mat4 M;

layout (location=0) in vec3 vs_vertexPosition;
layout (location=1) in vec3 vs_vertexColor;

void main() {
  const float w               = 1.f;
  gl_Position                 = vec4(vec4(M * vec4(vs_vertexPosition,w)).xy,0.f,w);
}


