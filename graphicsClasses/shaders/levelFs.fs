#version 330 core

uniform sampler2D depthTexture;

uniform mat4 VP;
uniform mat4 VPStar;

uniform vec3 positionCamera;
uniform light {
    vec3 directionLight;
    vec3 ambientLightIntensity;
    vec3 diffuseLightIntensity;
    vec3 specularLightIntensity;
};

uniform float burningCoeff;
in vec3 fs_vertexColor;
in vec4 fs_vertexDepthMapSpace;
in vec3 fs_vertexNormal;
in vec3 fs_vertexPositionWorld;

out vec4 pixelColor;


void main() {

    const mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
                                 0.0, 0.5, 0.0, 0.0,
                                 0.0, 0.0, 0.5, 0.0,
                                 0.5, 0.5, 0.5, 1.0);


    bool inShadow;
    vec4 fragStarSysCoord = biasMatrix * fs_vertexDepthMapSpace;

    if (texture(depthTexture, fragStarSysCoord.xy).x < ((fragStarSysCoord.z) - 0.001))
        inShadow = true;
    else {
        inShadow = false;
    }

    const vec3 fireEffet = vec3(10.0, 0.2, 0.0);

    vec3 ambientComponent = ambientLightIntensity;
    vec3 composition = ambientComponent *
                       ((1.0 - burningCoeff) * fs_vertexColor + (burningCoeff) * fireEffet);

    if (!inShadow) {

        vec3 toLight            = -directionLight;
        vec3 toCamera           = normalize(positionCamera - fs_vertexPositionWorld);
        vec3 reflection         =
            normalize(-toLight + 2.0 * (dot(toLight, fs_vertexNormal)) * fs_vertexNormal);
        vec3 diffuseComponent   = diffuseLightIntensity * max(0.0, dot(toLight, fs_vertexNormal));

        vec3 specularComponent  = specularLightIntensity * pow(max(0.0, dot(reflection, toCamera)), 20.0);

        composition  += diffuseComponent + specularComponent;
    }
    pixelColor = vec4(composition, 1.0);

}
