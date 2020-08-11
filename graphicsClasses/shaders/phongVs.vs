#version 330 core

uniform mat4  M;
uniform mat4  SR;

uniform mat4  W;
uniform mat4  VP;
uniform mat4  VPStar;

layout (location=0) in vec3 vs_vertexPosition;
layout (location=1) in vec3 vs_vertexColor;
layout (location=2) in vec3 vs_vertexNormal;

out vec3 fs_vertexColor;
out vec3 fs_vertexNormal;
out vec3 fs_vertexPositionWorld;
out vec4 fs_vertexDepthMapSpace;


void main() {
  const float w               = 1.f;

  vec4 posWorldSpace          = W * M * vec4(vs_vertexPosition,1.f);

  fs_vertexColor              = vs_vertexColor;
  fs_vertexNormal             = normalize((SR * vec4(vs_vertexNormal,1.f)).xyz);
  fs_vertexPositionWorld      = posWorldSpace.xyz;
  fs_vertexDepthMapSpace      = VPStar * W * M * vec4(vs_vertexPosition,w);
  gl_Position                 = VP * W * M * vec4(vs_vertexPosition,w);
}

