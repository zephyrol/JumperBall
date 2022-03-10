#version 300 es
precision highp float;

uniform sampler2D textureScene;

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
                       scalar * (1.0 - CIExyYColor.x - CIExyYColor.y));
    return XYZToRGB * CIEXYZ;
}

float getLuminance (vec3 xyYColor) {
    return xyYColor.z;
}

void main() {
    const float threshold = 4.0;
    vec3 colorRGB = texture(textureScene, fs_vertexUVs).xyz;
    vec3 colorxyY = convertRGBToCIExyY(colorRGB);
    if (getLuminance(colorxyY) > threshold) {
        pixelColor = vec4(convertCIExyYToRGB(colorxyY), 1.0);
    } else {
        pixelColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

}
