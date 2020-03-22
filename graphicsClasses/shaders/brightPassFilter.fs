#version 330 core

uniform sampler2D frameTexture;
uniform float     threshold;

in      vec2      fs_vertexUVs;
out     vec4      pixelColor;


const mat3  RGBToXYZ  = mat3 ( 2.7689, 1.7517,   1.1302,
                        1.0000, 4.5907,   0.060100,
                        0.0000, 0.056508, 5.5943);

vec3 convertRBGToCIExyY(vec3 rbgColor){

    vec3 CIEXYZ = RGBToXYZ * rbgColor;
    float sumXYZ = CIEXYZ.x + CIEXYZ.y + CIEXYZ.z;
    return vec3 ( CIEXYZ.x/sumXYZ, CIEXYZ.y/sumXYZ, CIEXYZ.y);
}

float getLuminance(vec3 xyYColor) {
    return xyYColor.z;
}

void brightPassFilter() {
  vec4 color = texture(frameTexture,fs_vertexUVs);
  vec3 colorxyY = convertRBGToCIExyY(color.xyz);

  if (getLuminance(colorxyY) > threshold) {
    pixelColor = color;
  } else {
    pixelColor = vec4(0.f,0.f,0.f,1.f);
  }
}

void main() {
  brightPassFilter();
}
