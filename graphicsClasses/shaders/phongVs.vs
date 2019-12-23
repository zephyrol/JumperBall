#version 330 core

uniform mat4  VP;
uniform mat4  MW;
uniform vec3  positionBall;
uniform vec3  lookDirection;
uniform float distanceBehind;

layout (location=0) in vec3 vs_vertexPosition;
layout (location=1) in vec3 vs_vertexColor;
layout (location=2) in vec3 vs_vertexNormal;

out vec3 fs_vertexColor;
out vec3 fs_vertexNormal;
out vec3 fs_vertexPositionWorld;
out float proximityObjectBehind;

void main() {
  const float minusInfinite   = -10000.f;
  const float w               = 1.f;

  vec4 posWorldSpace          = MW * vec4(vs_vertexPosition,1.f);
  vec3 ballToVertex           = vec3(posWorldSpace) - positionBall;

  bool behindCam  =  ( dot(lookDirection,ballToVertex) < -distanceBehind );
  if (behindCam)
      proximityObjectBehind   = minusInfinite;
  else proximityObjectBehind  = 1.f;

  fs_vertexColor              = vs_vertexColor;
  fs_vertexNormal             = vs_vertexNormal;
  fs_vertexPositionWorld      = (MW * vec4(vs_vertexPosition,1.f)).xyz;
  gl_Position                 = VP * MW * vec4(vs_vertexPosition,w);
}

