uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

uniform sampler2D frameSceneHDRTexture;
uniform sampler2D frameBluredTexture;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

const mat3 XYZToRGB = mat3(0.41847, -0.15866, -0.082835,
                           -0.091169, 0.25243, 0.015708,
                           0.00092090, -0.0025498, 0.17860);

const mat3 RGBToXYZ = mat3(2.7689, 1.7517,   1.1302,
                           1.0000, 4.5907,   0.060100,
                           0.0000, 0.056508, 5.5943);

vec3 convertCIExyYToRGB (vec3 CIExyYColor) {
    float scalar = CIExyYColor.z / CIExyYColor.y;
    vec3 CIEXYZ = vec3(scalar * CIExyYColor.x, CIExyYColor.z,
                       scalar * (1.0 - CIExyYColor.x - CIExyYColor.y));
    return XYZToRGB * CIEXYZ;
}

vec3 convertRBGToCIExyY (vec3 rbgColor) {
    vec3 CIEXYZ = RGBToXYZ * rbgColor;
    float sumXYZ = CIEXYZ.x + CIEXYZ.y + CIEXYZ.z;
    return vec3(CIEXYZ.x / sumXYZ, CIEXYZ.y / sumXYZ, CIEXYZ.y);
}


float getLuminance (vec3 xyYColor) {
    return xyYColor.z;
}

vec3 toneMappingOperator (vec3 xyYColor) {

    const float exposureLevelKey = 2.0;
    // const float luminanceWhite   = 6.39891f;
    // const float luminanceWhite   = 1.5f;

    const float averageLuminance = 1.8;

    float luminanceAfterToonMapping = exposureLevelKey * xyYColor.z
                                      / averageLuminance;
    /*float compressedLuminance = luminanceAfterToonMapping
     * ( 1.f + (luminanceAfterToonMapping / (whiteLuminance*whiteLuminance)))
       / ( 1.f + luminanceAfterToonMapping );
       xyYColor.z = compressedLuminance ;*/

    xyYColor.z = luminanceAfterToonMapping;

    return convertCIExyYToRGB(xyYColor);
}

void main() {
    vec4 baseRGBColor = texture(frameSceneHDRTexture, fs_vertexUVs);
    vec3 basexyYColor = convertRBGToCIExyY(baseRGBColor.xyz);
    vec3 toneMappedRGBColor = toneMappingOperator(basexyYColor);
    pixelColor = vec4(toneMappedRGBColor, baseRGBColor.a) + texture(frameBluredTexture, fs_vertexUVs);
    pixelColor = mix(pixelColor, vec4(flashColor, 1.0), teleportationCoeff);
}
