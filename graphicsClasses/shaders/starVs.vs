#version 330 core

uniform mat4  VP;
uniform mat4  MW;
uniform float radiusInside;
uniform float radiusOutside;
uniform vec3  colorInside;
uniform vec3  colorOutside;

layout (location=0) in vec3 vs_vertexPosition;

out vec3  fs_colorInside;
out vec3  fs_colorOutside;

out float fs_radiusInside;
out float fs_radiusOutside;

out vec3  fs_vertexPosition;

void main() {
  const float w     = 1.f;
  fs_colorInside    = colorInside;
  fs_colorOutside   = colorOutside;
  fs_radiusInside   = radiusInside;
  fs_radiusOutside  = radiusOutside;
  fs_vertexPosition = vs_vertexPosition; 
  gl_Position       = VP * MW * vec4(vs_vertexPosition,w);
}

