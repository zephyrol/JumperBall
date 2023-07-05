uniform sampler2D brightPassTexture;

const int patchSize = 25;
uniform vec2 offsetsAndGaussWeights[patchSize];

in vec2 fs_vertexUVs;
out vec4 pixelColor;

vec4 convertOutput(vec3 composition) {
    float compositionLength = length(composition);
    if(compositionLength < 1.0) {
        return vec4(composition, 0.0);
    }
    return vec4(
        normalize(composition),
        log2(compositionLength) / 3.0 // 3 because 2^3 = 8, its the max length
    );
}

vec3 convertInput(vec4 scenePixel) {
    if(scenePixel.a == 0.0) {
        return scenePixel.xyz;
    }
    float length = exp2(scenePixel.a * 3.0);
    return scenePixel.xyz * length;
}

void main() {
    vec3 blurColor = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < patchSize; ++i) {
        vec2 offsetAndGaussWeight = offsetsAndGaussWeights[i];
        vec2 neighboringPixelUV = fs_vertexUVs + vec2(offsetAndGaussWeight.x, 0.0);
        vec3 inputRGB = convertInput(texture(brightPassTexture, neighboringPixelUV));
        blurColor += offsetAndGaussWeight.y * inputRGB;
    }
    pixelColor = convertOutput(blurColor);
}
