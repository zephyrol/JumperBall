#version 330 core


uniform mat4  M;
uniform mat4  SR;

uniform mat4  W;
uniform mat4  VP;

layout (location=0) in vec3 vs_vertexPosition;

void main() {
  const float w               = 1.f;
  gl_Position                 = VP * W * M * vec4(vs_vertexPosition,w);
}

