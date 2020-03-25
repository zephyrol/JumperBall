#version 330 core


uniform sampler2D frameTexture;
uniform float     averageLuminance;
uniform float     whiteLuminance;

in      vec2      fs_vertexUVs;

out     vec4      pixelColor;

const mat3  RGBToXYZ  = mat3 ( 2.7689, 1.7517,   1.1302,
                        1.0000, 4.5907,   0.060100,
                        0.0000, 0.056508, 5.5943);
const mat3 XYZToRGB = inverse(RGBToXYZ);
                                    

vec3 convertCIExyYToRGB(vec3 CIExyYColor){


    float scalar = CIExyYColor.z / CIExyYColor.y;
    vec3 CIEXYZ = vec3 (scalar * CIExyYColor.x, CIExyYColor.z,
                            scalar* (1.f-CIExyYColor.x-CIExyYColor.y));
    return XYZToRGB * CIEXYZ;
}

vec3 convertRBGToCIExyY(vec3 rbgColor){

    vec3 CIEXYZ = RGBToXYZ * rbgColor;
    float sumXYZ = CIEXYZ.x + CIEXYZ.y + CIEXYZ.z;
    return vec3 ( CIEXYZ.x/sumXYZ, CIEXYZ.y/sumXYZ, CIEXYZ.y);
}

float getLuminance(vec3 xyYColor) {
    return xyYColor.z;
}


vec3 toneMappingOperator ( vec3 rgbColor ) {

    const float exposureLevelKey = 2.f;
    //const float luminanceWhite   = 6.39891f;
    //const float luminanceWhite   = 1.5f;

    vec3 xyYColor = convertRBGToCIExyY ( rgbColor );
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
