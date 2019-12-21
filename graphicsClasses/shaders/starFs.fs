#version 330 core

out vec4 pixelColor;

in vec3  fs_colorInside;
in vec3  fs_colorOutside;

in float fs_radiusInside;
in float fs_radiusOutside;

in vec3  fs_vertexPosition;

void main() {
    float dx = fs_vertexPosition.x - 0.5f;
    float dy = fs_vertexPosition.y - 0.5f;
    float dist = sqrt ( dx * dx + dy * dy );

    pixelColor = mix (  vec4(fs_colorInside,1.f), vec4(fs_colorOutside,0.f),
                        smoothstep ( fs_radiusInside, fs_radiusOutside, dist )); 
}