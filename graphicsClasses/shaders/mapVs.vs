#version 330 core

uniform mat4 VP;
uniform mat4 MW;
uniform vec3 positionBall;
uniform vec3 lookDirection;

layout (location=0) in vec3 vs_vertexPosition;
layout (location=1) in vec3 vs_vertexColor;

out vec4 fs_vertexColor;

void main() {
  float w = 1.f;
  vec4 posWorldSpace = MW * vec4(vs_vertexPosition,1.f);
  vec3 ballToVertex = vec3(posWorldSpace) - positionBall;
  bool behindCam =  ( dot(lookDirection,ballToVertex) < -0.5f );
  float alpha;
  if (behindCam)
      alpha = 0.f;
  else alpha = 1.f;
  fs_vertexColor = vec4(vs_vertexColor,alpha);
  gl_Position = VP * MW * vec4(vs_vertexPosition,w);
}

