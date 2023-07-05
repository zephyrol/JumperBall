uniform sampler2D horizontalBlurTexture;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

const int patchSize = 25;
uniform vec2 offsetsAndGaussWeights[patchSize];

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
        vec2 neighboringPixelUV = fs_vertexUVs + vec2(0.0, offsetAndGaussWeight.x);
        vec3 rgbColor = convertInput(texture(horizontalBlurTexture, neighboringPixelUV));
        blurColor += offsetAndGaussWeight.y * rgbColor;
    }
    pixelColor = vec4(blurColor, 0.0);
}
