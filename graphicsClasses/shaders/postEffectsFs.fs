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

uniform sampler2D sceneTexture;
uniform sampler2D postProcessTexture;

const int patchSize = 50;
const int verticalBlurFirstIndex = 25;
uniform vec2 offsetsAndGaussWeights[patchSize];
uniform int postProcessId;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

const mat3 RGBToXYZ = mat3(
    2.7689, 1.7517, 1.1302,
    1.0000, 4.5907, 0.060100,
    0.0000, 0.056508, 5.5943
);

const mat3 XYZToRGB = mat3(
    0.41847, -0.15866, -0.082835,
    -0.091169, 0.25243, 0.015708,
    0.00092090, -0.0025498, 0.17860
);

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

vec3 convertInput(vec4 scenePixel) {
    if(scenePixel.a == 0.0) {
        return scenePixel.xyz;
    }
    float length = exp2(scenePixel.a * 3.0);
    return scenePixel.xyz * length;
}

vec4 convertOutput(vec3 composition) {
    float compositionLength = length(composition);
    if(compositionLength < 1.0) {
        return vec4(composition, 0.0);
    }
    return vec4(
        normalize(composition),
        log2(compositionLength) * 0.333333 // divide by 3 because 2^3 = 8, its the max length
    );
}

vec3 toneMappingOperator (vec3 xyYColor) {

    const float exposureLevelKey = 2.0;
    const float averageLuminance = 1.8;

    float luminanceAfterToneMapping = exposureLevelKey * xyYColor.z
    / averageLuminance;

    xyYColor.z = luminanceAfterToneMapping;

    return convertCIExyYToRGB(xyYColor);
}


vec4 getBrightPassFilterColor() {
    const float threshold = 4.0;
    vec4 scenePixel = texture(sceneTexture, fs_vertexUVs);
    vec3 colorRGB = convertInput(scenePixel);
    vec3 colorxyY = convertRGBToCIExyY(colorRGB);
    // Check if luminance is greater than the threshold.
    if (colorxyY.z > threshold) {
        return scenePixel;
    }
    return vec4(0.0);
}

vec4 getHorizontalBlurColor() {
    vec3 blurColor = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < verticalBlurFirstIndex; ++i) {
        vec2 offsetAndGaussWeight = offsetsAndGaussWeights[i];
        vec2 neighboringPixelUV = fs_vertexUVs + vec2(offsetAndGaussWeight.x, 0.0);
        vec3 inputRGB = convertInput(texture(postProcessTexture, neighboringPixelUV));
        blurColor += offsetAndGaussWeight.y * inputRGB;
    }
    return convertOutput(blurColor);
}

vec4 getVerticalBlurColor() {
    vec3 blurColor = vec3(0.0, 0.0, 0.0);
    for (int i = verticalBlurFirstIndex; i < patchSize; ++i) {
        vec2 offsetAndGaussWeight = offsetsAndGaussWeights[i];
        vec2 neighboringPixelUV = fs_vertexUVs + vec2(0.0, offsetAndGaussWeight.x);
        vec3 inputRGB = convertInput(texture(postProcessTexture, neighboringPixelUV));
        blurColor += offsetAndGaussWeight.y * inputRGB;
    }
    return vec4(blurColor, 0.0);
}

vec4 getBloomColor() {
    vec3 baseRGBColor = convertInput(texture(sceneTexture, fs_vertexUVs));
    vec3 basexyYColor = convertRGBToCIExyY(baseRGBColor);
    vec3 toneMappedRGBColor = toneMappingOperator(basexyYColor);
    vec3 pixelColorVec3 = toneMappedRGBColor + texture(postProcessTexture, fs_vertexUVs).xyz;
    return vec4(mix(pixelColorVec3, flashColor, teleportationCoeff), 1.0);
}

void main() {
    // Bloom is tested first because it's applied on more pixels.
    if (postProcessId == 3) {
        pixelColor = getBloomColor();
    } else if(postProcessId == 0) {
        pixelColor = getBrightPassFilterColor();
    } else if (postProcessId == 1) {
        pixelColor = getHorizontalBlurColor();
    }
    // Pass id = 2
    pixelColor = getVerticalBlurColor();
}
