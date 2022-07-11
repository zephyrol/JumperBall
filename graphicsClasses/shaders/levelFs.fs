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

const float shadowTextureSize = 1024.0;

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

float evaluateShadowOffset(vec4 vertexDepthMapSpace, float offsetX, float offsetY, int textureNumber) {
    vec2 shadowCoord = vec2(
        vertexDepthMapSpace.x + offsetX / shadowTextureSize,
        vertexDepthMapSpace.y + offsetY / shadowTextureSize
    );
    if(textureNumber == 0) {
        if (texture(depthTexture, shadowCoord.xy).x < vertexDepthMapSpace.z) {
            return 0.0;
        }
        return 0.25;
    }
    if (texture(depth2Texture, shadowCoord.xy).x < vertexDepthMapSpace.z) {
        return 0.0;
    }
    return 0.25;
}

float evaluateShadow(vec4 vertexDepthMapSpace, int textureNumber) {
    float shadowCoeff = 0.0;
    shadowCoeff += evaluateShadowOffset(vertexDepthMapSpace, 1.0, 1.0, textureNumber);
    shadowCoeff += evaluateShadowOffset(vertexDepthMapSpace, -1.0, 1.0, textureNumber);
    shadowCoeff += evaluateShadowOffset(vertexDepthMapSpace, 1.0, -1.0, textureNumber);
    shadowCoeff += evaluateShadowOffset(vertexDepthMapSpace, -1.0, -1.0, textureNumber);
    return shadowCoeff;
}

void main(){

    bool inFirstShadow;

    float firstShadowCoeff = evaluateShadow(fs_vertexDepthMapSpace, 0);
    float secondShadowCoeff = evaluateShadow(fs_vertexDepthMap2Space, 1);

    const vec3 fireEffet = vec3(10.0, 0.2, 0.0);

    vec3 ambientComponent = ambientLightIntensity;
    vec3 composition = ambientComponent * (
        (1.0 - burningCoeff) * fs_vertexColor + burningCoeff * fireEffet
    );

    if (!(firstShadowCoeff == 0.0 && secondShadowCoeff == 0.0)) {

        vec3 toCamera = normalize(cameraPosition - fs_vertexPositionWorld);
        if(firstShadowCoeff != 0.0) {
            composition += firstShadowCoeff * getLightContribution(
                toCamera,
                lightDirection,
                diffuseLightIntensity,
                specularLightIntensity
            );
        }

        if(secondShadowCoeff != 0.0) {
            composition += secondShadowCoeff * getLightContribution(
                toCamera,
                light2Direction,
                diffuseLight2Intensity,
                specularLight2Intensity
            );
        }

    }
    pixelColor = vec4(composition, 1.0);
}
