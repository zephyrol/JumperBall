#version 330 core

uniform sampler2D frameTexture;
uniform bool firstPass;
uniform int patchSize;
uniform float[25]  gaussWeights;

in vec2 fs_vertexUVs;
out vec4 pixelColor;


void computeBlur (bool horizontal) {

    const int indicesOffset[25] =
        int[] (-12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0,
               1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

    const int levelOfDetail = 0;

    float sumCoefficients = 0;
    vec3 blurColor = vec3(0.f, 0.f, 0.f);
    vec2 texelOffset = 1.f / textureSize(frameTexture, 0);
    if (horizontal) {
        for (int i = 0; i < 25; i++) {
            float coefficient = gaussWeights[i];
            vec2 neighboringPixelUV = fs_vertexUVs +
                                      vec2(texelOffset.x * indicesOffset[i], 0.f);

            blurColor += coefficient *
                         texture(frameTexture, neighboringPixelUV).xyz;
            sumCoefficients += coefficient;
        }
    } else {
        for (int i = 0; i < 25; i++) {
            float coefficient = gaussWeights[i];

            vec2 neighboringPixelUV = fs_vertexUVs +
                                      vec2(0.f, texelOffset.y * indicesOffset[i]);
            blurColor += coefficient *
                         texture(frameTexture, neighboringPixelUV).xyz;
            sumCoefficients += coefficient;
        }
    }
    blurColor /= sumCoefficients;
    pixelColor = vec4(blurColor, 1.f);

}

void horizontalPass() {

    computeBlur(true);
}

void verticalPass() {

    computeBlur(false);
}

void main() {
    if (firstPass) {
        horizontalPass();
    } else {
        verticalPass();
    }
}
