#version 330 core

uniform vec3  positionCamera;

uniform light {
              vec3  positionLight;
              vec3  ambiantLightIntensity;
              vec3  diffuseLightIntensity;
              vec3  specularLightIntensity;
              };

out vec4  pixelColor;
in vec3   fs_vertexColor;
in vec3   fs_vertexNormal;
in vec3   fs_vertexPositionWorld;
in float  proximityObjectBehind;


void main() {
    const float epsilon     = 0.99999f;
    if (proximityObjectBehind < epsilon ) discard;

    vec3 toLight            = normalize(positionLight 
                                          - fs_vertexPositionWorld);
    vec3 toCamera           = normalize(positionCamera 
                                          - fs_vertexPositionWorld);
    vec3 reflection         = -toLight + 2.f * ( dot (toLight, fs_vertexNormal)) 
                                * fs_vertexNormal;
    reflection              = normalize(reflection);

    vec3 ambiantComponent   = ambiantLightIntensity;

    vec3 diffuseComponent   = diffuseLightIntensity * 
                              max(0.f,dot(toLight,fs_vertexNormal));

    
    vec3 specularComponent  = specularLightIntensity * 
                              pow(max(0.f,dot(reflection,toCamera)),20.f);

    vec3 composition = ambiantComponent * fs_vertexColor + diffuseComponent
                        + specularComponent;
    pixelColor = vec4(composition,1.f);
                  
                  
}