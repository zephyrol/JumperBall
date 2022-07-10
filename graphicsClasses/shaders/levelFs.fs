uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 light2Direction;
    vec3 flashColor;
    float teleportationCoeff;
};

uniform float burningCoeff;
uniform sampler2D depthTexture;
uniform sampler2D depth2Texture;

in vec3 fs_vertexColor;
in vec4 fs_vertexDepthMapSpace;
in vec4 fs_vertexDepthMap2Space;
in vec3 fs_vertexNormal;
in vec3 fs_vertexPositionWorld;

out vec4 pixelColor;

const vec3 ambientLightIntensity = vec3(0.7, 0.7, 0.7);

const vec3 diffuseLightIntensity = vec3(0.20, 0.25, 0.25);
const vec3 specularLightIntensity = vec3(0.0, 0.25, 0.25);

const vec3 diffuseLight2Intensity = vec3(0.25, 0.20, 0.25);
const vec3 specularLight2Intensity = vec3(0.25, 0.0, 0.25);

vec3 getLightContribution(vec3 toCamera, vec3 lightDir, vec3 diffuseLightInt, vec3 specularLightInt) {
    vec3 toLight = -lightDir;
    float dotToLightVertexNormal = dot(toLight, fs_vertexNormal);
    vec3 reflection = normalize(-toLight + 2.0 * dotToLightVertexNormal * fs_vertexNormal);
    float croppedDot = max(0.0, dotToLightVertexNormal);
    vec3 diffuseComponent = diffuseLightInt * croppedDot;
    vec3 specularComponent = specularLightInt * pow(
        max(0.0, dot(reflection, toCamera)),
        20.0
    );
    return diffuseComponent + specularComponent;
}

void main() {

    float zThreshold = fs_vertexDepthMapSpace.z - 0.001;
    bool inFirstShadow;
    if (texture(depthTexture, fs_vertexDepthMapSpace.xy).x < zThreshold) {
        inFirstShadow = true;
    } else {
        inFirstShadow = false;
    }

    bool inSecondShadow;
    float zThreshold2 = fs_vertexDepthMap2Space.z - 0.001;
    if (texture(depth2Texture, fs_vertexDepthMap2Space.xy).x < zThreshold2) {
        inSecondShadow = true;
    } else {
        inSecondShadow = false;
    }

    const vec3 fireEffet = vec3(10.0, 0.2, 0.0);

    vec3 ambientComponent = ambientLightIntensity;
    vec3 composition = ambientComponent * (
        (1.0 - burningCoeff) * fs_vertexColor + burningCoeff * fireEffet
    );

    if (!(inFirstShadow && inSecondShadow)) {

        vec3 toCamera = normalize(cameraPosition - fs_vertexPositionWorld);
        if(!inFirstShadow) {
            composition += getLightContribution(
                toCamera,
                lightDirection,
                diffuseLightIntensity,
                specularLightIntensity
            );
        }

        if(!inSecondShadow) {
            composition += getLightContribution(
                toCamera,
                light2Direction,
                diffuseLight2Intensity,
                specularLight2Intensity
            );
        }

    }
    pixelColor = vec4(composition, 1.0);
}
