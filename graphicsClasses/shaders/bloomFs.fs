uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 light2Direction;
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

vec3 convertCIExyYToRGB (vec3 CIExyYColor) {
    float scalar = CIExyYColor.z / CIExyYColor.y;
    vec3 CIEXYZ = vec3(scalar * CIExyYColor.x, CIExyYColor.z,
                       scalar * (1.0 - CIExyYColor.x - CIExyYColor.y));
    return XYZToRGB * CIEXYZ;
}

float getLuminance (vec3 xyYColor) {
    return xyYColor.z;
}

vec3 toneMappingOperator (vec3 xyYColor) {

    const float exposureLevelKey = 2.0;
    // const float luminanceWhite   = 6.39891f;
    // const float luminanceWhite   = 1.5f;

    const float averageLuminance = 1.8;

    float luminanceAfterToneMapping = exposureLevelKey * xyYColor.z
                                      / averageLuminance;
    /*float compressedLuminance = luminanceAfterToneMapping
     * ( 1.f + (luminanceAfterToneMapping / (whiteLuminance*whiteLuminance)))
       / ( 1.f + luminanceAfterToneMapping );
       xyYColor.z = compressedLuminance ;*/

    xyYColor.z = luminanceAfterToneMapping;

    return convertCIExyYToRGB(xyYColor);
}

void main() {
    vec3 basexyYColor = texture(frameSceneHDRTexture, fs_vertexUVs).xyz;
    vec3 toneMappedRGBColor = toneMappingOperator(basexyYColor);
    vec3 pixelColorVec3 = toneMappedRGBColor + texture(frameBluredTexture, fs_vertexUVs).xyz;
    pixelColor = vec4(mix(pixelColorVec3, flashColor, teleportationCoeff), 1.0);
}
