#version 330 core

uniform mat4  VP;
uniform mat4  MW;
uniform vec3  positionBall;
uniform vec3  lookDirection;
uniform float distanceBehind;

layout (location=0) in vec3 vs_vertexPosition;

out vec3 fs_vertexColor;

void main() {
  float w = 1.f;
  //fs_vertexColor  = vs_vertexColor;
  gl_Position     = VP * MW * vec4(vs_vertexPosition,w);
}

