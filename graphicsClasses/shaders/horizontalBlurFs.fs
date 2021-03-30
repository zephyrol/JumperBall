#version 330 core

uniform sampler2D brightPassTexture;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

void main() {

    const int patchSize = 25;
    const int indicesOffset[25] =
        int[] (-12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

    // Getting 25 Gauss weights computed with sigma = 4
    const float gaussWeights[25] = float[] (0.00110796, 0.00227339, 0.00438208, 0.00793491, 0.0134977,
                                            0.0215693, 0.0323794, 0.0456623, 0.0604927, 0.0752844, 0.0880163,
                                            0.096667, 0.0997356, 0.096667,
                                            0.0880163, 0.0752844, 0.0604927, 0.0456623, 0.0323794, 0.0215693,
                                            0.0134977, 0.00793491,
                                            0.00438208, 0.00227339, 0.00110796);

    const int levelOfDetail = 0;

    float sumCoefficients = 0;
    vec3 blurColor = vec3(0.0, 0.0, 0.0);
    vec2 texelOffset = 1.0 / textureSize(brightPassTexture, 0);
    for (int i = 0; i < patchSize; i++) {
        float coefficient = gaussWeights[i];
        vec2 neighboringPixelUV = fs_vertexUVs + vec2(texelOffset.x * indicesOffset[i], 0.0);
        blurColor += coefficient * texture(brightPassTexture, neighboringPixelUV).xyz;
        sumCoefficients += coefficient;
    }
    blurColor /= sumCoefficients;
    pixelColor = vec4(blurColor, 1.0);
}
