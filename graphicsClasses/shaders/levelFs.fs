#version 300 es

uniform Scene {
    mat4 VP;
    mat4 VPStar;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

uniform float burningCoeff;
uniform sampler2D depthTexture;

in vec3 fs_vertexColor;
in vec4 fs_vertexDepthMapSpace;
in vec3 fs_vertexNormal;
in vec3 fs_vertexPositionWorld;

out vec4 pixelColor;

const vec3 ambientLightIntensity = vec3(0.7, 0.7, 0.7);
const vec3 diffuseLightIntensity = vec3(0.25, 0.25, 0.25);
const vec3 specularLightIntensity = vec3(0.25, 0.25, 0.25);

void main() {

    const mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
                                 0.0, 0.5, 0.0, 0.0,
                                 0.0, 0.0, 0.5, 0.0,
                                 0.5, 0.5, 0.5, 1.0);


    vec4 fragStarSysCoord = biasMatrix * fs_vertexDepthMapSpace;

    bool inShadow;
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

        vec3 toLight = -lightDirection;
        vec3 toCamera = normalize(cameraPosition - fs_vertexPositionWorld);
        vec3 reflection = normalize(-toLight + 2.0 * (dot(toLight, fs_vertexNormal)) * fs_vertexNormal);
        vec3 diffuseComponent = diffuseLightIntensity * max(0.0, dot(toLight, fs_vertexNormal));

        vec3 specularComponent = specularLightIntensity * pow(max(0.0, dot(reflection, toCamera)), 20.0);

        composition += diffuseComponent + specularComponent;
    }
    pixelColor = vec4(composition, 1.0);
}
