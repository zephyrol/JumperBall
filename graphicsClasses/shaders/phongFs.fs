#version 330 core

uniform vec3  positionCamera;
uniform sampler2D depthTexture;

uniform mat4  VP;
uniform mat4  VPStar;

uniform light {
              vec3  positionLight;
              vec3  ambiantLightIntensity;
              vec3  diffuseLightIntensity;
              vec3  specularLightIntensity;
              };

in vec3   fs_vertexColor;
in vec3   fs_vertexNormal;
in vec3   fs_vertexPositionWorld;
in vec4   fs_vertexDepthMapSpace;
in float  proximityObjectBehind;

out vec4  pixelColor;


mat4 VPinverse = inverse(VP);

void main() {
    const float epsilon     = 0.99999f;

    const mat4 biasMatrix   = mat4(0.5f,0.f, 0.f, 0.f,
                               0.f, 0.5f,0.f, 0.f,
                               0.f, 0.f, 0.5f,0.f,
                               0.5f, 0.5f, 0.5f, 1.f);

    if (proximityObjectBehind < epsilon ) discard;

    bool inShadow;
    vec4 fragStarSysCoord   = biasMatrix * fs_vertexDepthMapSpace;
    
    if ( texture(depthTexture, fragStarSysCoord.xy).x
          < ((fragStarSysCoord.z) - 0.001) )
        inShadow = true;
    else {
        inShadow = false;
    }

    vec3 ambiantComponent   = ambiantLightIntensity;
    vec3 composition = ambiantComponent * fs_vertexColor;
    
    if (!inShadow) {
        vec3 toLight            = normalize(positionLight 
                                              - fs_vertexPositionWorld);
        vec3 toCamera           = normalize(positionCamera 
                                              - fs_vertexPositionWorld);

        vec3 reflection         = -toLight + 2.f * 
                                  ( dot (toLight, fs_vertexNormal)) 
                                  * fs_vertexNormal;

        reflection              = normalize(reflection);

        vec3 diffuseComponent   = diffuseLightIntensity *
                                  max(0.f,dot(toLight,fs_vertexNormal));
        
        vec3 specularComponent  = specularLightIntensity *
                                  pow(max(0.f,dot(reflection,toCamera)),20.f);

        composition             += diffuseComponent + specularComponent;
    }
    pixelColor = vec4(composition,1.f);

    //pixelColor = texture(depthTexture, fragStarSysCoord.xy);
                  
}
