#version 330 core

uniform sampler2D frameTexture;
uniform bool      firstPass;
uniform int       sizePatch;
in      vec2      fs_vertexUVs;
out     vec4      pixelColor;

float evalGauss1D(float x, float sigma) {
    const float PI = 3.14159265;
    return exp((-pow(x,2.))/(2.*pow(sigma,2.)))/(sqrt(2.* PI * pow(sigma,2.)));
}


void horizontalPass() {
  
    float coefficientH ;
    ivec2 posPixel = ivec2(gl_FragCoord.xy);
    const int levelOfDetail = 0;
    
    
    float sumCoefficients = 0;
    pixelColor = vec4(0.f,0.f,0.f,0.f);

    for (int i = -sizePatch/2; i <= sizePatch/2 ; i++ ) {
        coefficientH = evalGauss1D(i,4.f);
        vec4 color = coefficientH * texelFetchOffset( 
            frameTexture,posPixel, levelOfDetail, ivec2(i,0));
        pixelColor += color;
        sumCoefficients += coefficientH;
    }
    pixelColor/=sumCoefficients;

}

void verticalPass() {

    float coefficientV ;
    ivec2 posPixel = ivec2(gl_FragCoord.xy);
    const int levelOfDetail = 0;
    
    float sumCoefficients = 0;
    pixelColor = vec4(0.f,0.f,0.f,0.f);

    for (int i = -sizePatch; i <= sizePatch/2 ; i++ ) {
        coefficientV = evalGauss1D(i,4.f);
        vec4 color = coefficientV * texelFetchOffset( 
            frameTexture,posPixel, levelOfDetail, ivec2(0,i));
        pixelColor += color;
        sumCoefficients += coefficientV;
    }
    pixelColor/=sumCoefficients;

}

void main() {
  if (firstPass) {
  horizontalPass();
  } else {
  verticalPass();
  }
}
