#version 330 core

uniform sampler2D frameTexture;
uniform bool      firstPass;
uniform int       patchSize;
uniform float[9]   gaussWeights;

in      vec2      fs_vertexUVs;
out     vec4      pixelColor;


void computeBlur(ivec2 factors){

    ivec2 posPixel = ivec2(gl_FragCoord.xy);
    const int levelOfDetail = 0;
    
    float sumCoefficients = 0;
    pixelColor = vec4(0.f,0.f,0.f,0.f);
    int counter = 0;
    int last = patchSize/2;
    for (int i = -patchSize/2; i <= last ; i++ ) {
        float coefficient = gaussWeights[counter];
        vec4 color = coefficient * texelFetchOffset( 
            frameTexture,posPixel, levelOfDetail, ivec2(i,i) * factors);
        pixelColor += color;
        sumCoefficients += coefficient;
        counter++;
    }
    pixelColor/=sumCoefficients;

}

void horizontalPass() {

  computeBlur(ivec2(1,0));
}

void verticalPass() {

  computeBlur(ivec2(0,1));
}

void main() {
  if (firstPass) {
    horizontalPass();
  } else {
    verticalPass();
  }
}
