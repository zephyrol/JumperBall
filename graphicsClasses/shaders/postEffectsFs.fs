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

const int patchSize = 34;
const int verticalBlurFirstIndex = 17;
uniform vec2 offsetsAndGaussWeights[patchSize];
uniform int postProcessId;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

vec3 convertInput(vec4 scenePixel) {
    if(scenePixel.a == 0.0) {
        return scenePixel.xyz;
    }
    float length = 1.0 + (scenePixel.a * 7.0); // 7.0 because the max length is 8 (1 + 7)
    return scenePixel.xyz * length;
}

vec4 convertOutput(vec3 composition) {
    float compositionLength = length(composition);
    if(compositionLength < 1.0) {
        return vec4(composition, 0.0);
    }
    return vec4(
        normalize(composition),
        (compositionLength - 1.0) / 7.0 // 7.0 because the max length is 8 (1 + 7)
    );
}

vec4 getBrightPassFilterColor() {
    const vec3 luminanceVector = vec3(0.2126, 0.7152, 0.0722);
    vec4 scenePixel = texture(sceneTexture, fs_vertexUVs);
    vec3 colorRGB = convertInput(scenePixel);
    // Check if luminance is greater than the threshold.
    if (dot(colorRGB, luminanceVector) > 0.6) {
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
    vec3 pixelColorVec3 = baseRGBColor + texture(postProcessTexture, fs_vertexUVs).xyz;
    return vec4(mix(pixelColorVec3, flashColor, teleportationCoeff), 1.0);
}

void main() {
    // Bloom is tested first because it's applied on more pixels.
    if (postProcessId == 3) {
        pixelColor = getBloomColor();
        return;
    }
    if(postProcessId == 0) {
        pixelColor = getBrightPassFilterColor();
        return;
    }
    if (postProcessId == 1) {
        pixelColor = getHorizontalBlurColor();
        return;
    }
    // Pass id = 2
    pixelColor = getVerticalBlurColor();
}
