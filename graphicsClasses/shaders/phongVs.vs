#version 330 core

uniform mat4  M;
uniform mat4  SR;

uniform mat4  W;
uniform mat4  VP;
uniform mat4  VPStar;

uniform vec3 positionCamera;
uniform light {
              vec3  directionLight;
              vec3  ambientLightIntensity;
              vec3  diffuseLightIntensity;
              vec3  specularLightIntensity;
              };


layout (location=0) in vec3 vs_vertexPosition;
layout (location=1) in vec3 vs_vertexColor;
layout (location=2) in vec3 vs_vertexNormal;

out vec3 fs_vertexColor;
out vec4 fs_vertexDepthMapSpace;
out vec3 fs_vertexDiffuseSpecular;


void main() {
  const float w               = 1.f;

  vec4 posWorldSpace          = W * M * vec4(vs_vertexPosition,1.f);
  fs_vertexColor              = vs_vertexColor;
  vec3 vertexNormal                = normalize((SR * vec4(vs_vertexNormal,1.f)).xyz);
  vec3 vertexPositionWorld      = posWorldSpace.xyz;
  fs_vertexDepthMapSpace      = VPStar * W * M * vec4(vs_vertexPosition,w);


  vec3 toLight            = -directionLight;
  vec3 toCamera           = normalize(positionCamera - vertexPositionWorld);

  vec3 reflection         = -toLight + 2.f * 
      ( dot (toLight, vertexNormal)) 
      * vertexNormal;

  reflection              = normalize(reflection);

  vec3 diffuseComponent   = diffuseLightIntensity *
      max(0.f,dot(toLight,vertexNormal));

  vec3 specularComponent  = specularLightIntensity *
      pow(max(0.f,dot(reflection,toCamera)),20.f);

  fs_vertexDiffuseSpecular    = diffuseComponent + specularComponent;

  gl_Position                 = VP * W * M * vec4(vs_vertexPosition,w);
}

