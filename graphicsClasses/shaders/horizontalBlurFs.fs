uniform sampler2D brightPassTexture;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

const int patchSize = 25;
const float indicesOffsets[patchSize] = float[] (
    -12.0,
    -11.0,
    -10.0,
    -9.0,
    -8.0,
    -7.0,
    -6.0,
    -5.0,
    -4.0,
    -3.0,
    -2.0,
    -1.0,
    0.0,
    1.0,
    2.0,
    3.0,
    4.0,
    5.0,
    6.0,
    7.0,
    8.0,
    9.0,
    10.0,
    11.0,
    12.0
);

// Getting 25 Gauss weights computed with sigma = 4
const float gaussWeights[25] = float[] (
    0.00110796, 0.00227339, 0.00438208, 0.00793491, 0.0134977, 0.0215693,
    0.0323794, 0.0456623, 0.0604927, 0.0752844, 0.0880163, 0.096667,
    0.0997356, 0.096667, 0.0880163, 0.0752844, 0.0604927, 0.0456623,
    0.0323794, 0.0215693, 0.0134977, 0.00793491, 0.00438208, 0.00227339, 0.00110796
);

void main() {

    const int levelOfDetail = 0;

    vec3 blurColor = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < patchSize; ++i) {
        float coefficient = gaussWeights[i];
        vec2 neighboringPixelUV = fs_vertexUVs + vec2(texelSize.x * indicesOffsets[i], 0.0);
        blurColor += coefficient * texture(brightPassTexture, neighboringPixelUV).xyz;
    }
    pixelColor = vec4(blurColor, 1.0);
}
