#version 330 core


uniform mat4  MW;
uniform mat4  VPStar;

layout (location=0) in vec3 vs_vertexPosition;

void main() {
  const float w               = 1.f;
  gl_Position                 = VPStar * MW * vec4(vs_vertexPosition,w);
}

