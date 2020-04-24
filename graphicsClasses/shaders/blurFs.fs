#version 330 core

uniform sampler2D frameTexture;
uniform bool      firstPass;
uniform int       patchSize;
uniform float[25]  gaussWeights;

in      vec2      fs_vertexUVs;
out     vec4      pixelColor;


void computeBlur(bool horizontal){

    const int pixelOffset [25] =
        int[](-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,
              1,2,3,4,5,6,7,8,9,10,11,12);

    const int levelOfDetail = 0;

    ivec2 posPixel = ivec2(gl_FragCoord.xy);
    float sumCoefficients = 0;
    pixelColor = vec4(0.f,0.f,0.f,0.f);
    if ( horizontal ) {
        for (int i = 0; i < 25 ; i++ ) {
            float coefficient = gaussWeights[i];
            pixelColor+= coefficient * texelFetchOffset(
                                        frameTexture,posPixel, levelOfDetail,
                                        ivec2(pixelOffset[i],0));
            sumCoefficients += coefficient;
        }
    }
    else {
        for (int i = 0; i < 25 ; i++ ) {
            float coefficient = gaussWeights[i];
            pixelColor+= coefficient * texelFetchOffset(
                    frameTexture,posPixel, levelOfDetail,
                    ivec2(0,pixelOffset[i]));
            sumCoefficients += coefficient;
        }
    }

    pixelColor/=sumCoefficients;

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
