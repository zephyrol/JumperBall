#version 330 core

out vec4 pixelColor;

in vec3  fs_colorInside;
in vec3  fs_colorOutside;

in float fs_radiusInside;
in float fs_radiusOutside;

in vec2  fs_vertexPosition;

void main() {

    // Star center at the position 0.f,0.f
    float dx          = fs_vertexPosition.x - 0.5f;
    float dy          = fs_vertexPosition.y - 0.5f;

    float dist        = sqrt ( dx * dx + dy * dy );
    float coefficient = smoothstep( fs_radiusInside, 
                                    fs_radiusOutside, 
                                    dist);

    pixelColor        = mix ( vec4(fs_colorInside*3.f,1.f), 
                              vec4(fs_colorOutside*3.f,0.f),
                              coefficient); 
}