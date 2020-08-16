#version 330 core


uniform sampler2D frameTexture;
uniform float     averageLuminance;
uniform float     whiteLuminance;

in      vec2      fs_vertexUVs;

out     vec4      pixelColor;

const mat3 XYZToRGB = mat3 (0.41847,-0.15866, -0.082835,
                            -0.091169,0.25243,0.015708,
                            0.00092090, -0.0025498, 0.17860);

vec3 convertCIExyYToRGB(vec3 CIExyYColor){
    float scalar = CIExyYColor.z / CIExyYColor.y;
    vec3 CIEXYZ = vec3 (scalar * CIExyYColor.x, CIExyYColor.z,
                            scalar* (1.f-CIExyYColor.x-CIExyYColor.y));
    return XYZToRGB * CIEXYZ;
}

float getLuminance(vec3 xyYColor) {
    return xyYColor.z;
}

vec3 toneMappingOperator (vec3 xyYColor) {

    const float exposureLevelKey = 2.f;
    //const float luminanceWhite   = 6.39891f;
    //const float luminanceWhite   = 1.5f;

    float luminanceAfterToonMapping = exposureLevelKey * xyYColor.z 
                                      / averageLuminance;
    /*float compressedLuminance = luminanceAfterToonMapping 
      * ( 1.f + (luminanceAfterToonMapping / (whiteLuminance*whiteLuminance)))
      / ( 1.f + luminanceAfterToonMapping );
    xyYColor.z = compressedLuminance ;*/

    xyYColor.z = luminanceAfterToonMapping;

    return convertCIExyYToRGB(xyYColor);
}


void main() {
    vec4 baseColor = texture(frameTexture,fs_vertexUVs);
    vec3 toneMappedColor = toneMappingOperator (baseColor.xyz);
    pixelColor = vec4(toneMappedColor,baseColor.a);
}
