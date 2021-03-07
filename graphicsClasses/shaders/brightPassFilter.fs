#version 330 core

uniform sampler2D frameTexture;
uniform float threshold;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

const mat3 RGBToXYZ = mat3(2.7689, 1.7517,   1.1302,
                           1.0000, 4.5907,   0.060100,
                           0.0000, 0.056508, 5.5943);

const mat3 XYZToRGB = mat3(0.41847, -0.15866, -0.082835,
                           -0.091169, 0.25243, 0.015708,
                           0.00092090, -0.0025498, 0.17860);

vec3 convertRGBToCIExyY (vec3 rbgColor) {

    vec3 CIEXYZ = RGBToXYZ * rbgColor;
    float sumXYZ = CIEXYZ.x + CIEXYZ.y + CIEXYZ.z;
    return vec3(CIEXYZ.x / sumXYZ, CIEXYZ.y / sumXYZ, CIEXYZ.y);
}

vec3 convertCIExyYToRGB (vec3 CIExyYColor) {
    float scalar = CIExyYColor.z / CIExyYColor.y;
    vec3 CIEXYZ = vec3(scalar * CIExyYColor.x, CIExyYColor.z,
                       scalar * (1.f - CIExyYColor.x - CIExyYColor.y));
    return XYZToRGB * CIEXYZ;
}

float getLuminance (vec3 xyYColor) {
    return xyYColor.z;
}

void main() {
    vec3 colorRGB = texture(frameTexture, fs_vertexUVs).xyz;
    vec3 colorxyY = convertRGBToCIExyY(colorRGB);
    if (getLuminance(colorxyY) > threshold) {
        pixelColor = vec4(convertCIExyYToRGB(colorxyY), 1.f);
    } else {
        pixelColor = vec4(0.f, 0.f, 0.f, 1.f);
    }

}
