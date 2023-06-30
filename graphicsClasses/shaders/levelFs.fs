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

vec3 getLightContribution(
    vec3 normalVec,
    vec3 toCamera,
    vec3 lightDir,
    vec3 diffuseLightIntensity,
    vec3 specularLightIntensity
) {
    vec3 reflection = normalize(reflect(lightDir, normalVec));
    vec3 diffuseComponent = diffuseLightIntensity * dot(-lightDir, normalVec);
    float dotReflectionToCamera = dot(reflection, toCamera);
    if(dotReflectionToCamera < 0.0) {
        return diffuseComponent;
    }
    vec3 specularComponent = specularLightIntensity * pow(
        dotReflectionToCamera,
        20.0
    );
    return diffuseComponent + specularComponent;
}

const float shadowPixelSize = 1.0 / shadowTextureSize;
const vec2 shadowOffset1 = vec2(shadowPixelSize, shadowPixelSize);
const vec2 shadowOffset2 = vec2(-shadowPixelSize, shadowPixelSize);
const vec2 shadowOffset3 = -shadowOffset1;
const vec2 shadowOffset4 = -shadowOffset2;

float evaluateShadow(vec4 vertexDepthMapSpace, sampler2D depthT) {
    float shadowCoeff = 0.0;
    vec2 vertexDepthMapSpaceXY = vertexDepthMapSpace.xy;
    if (texture(depthT, vertexDepthMapSpaceXY + shadowOffset1).x > vertexDepthMapSpace.z) {
        shadowCoeff += 0.25;
    }
    if (texture(depthT, vertexDepthMapSpaceXY + shadowOffset2).x > vertexDepthMapSpace.z) {
        shadowCoeff += 0.25;
    }
    if (texture(depthT, vertexDepthMapSpaceXY + shadowOffset3).x > vertexDepthMapSpace.z) {
        shadowCoeff += 0.25;
    }
    if (texture(depthT, vertexDepthMapSpaceXY + shadowOffset4).x > vertexDepthMapSpace.z) {
        shadowCoeff += 0.25;
    }
    return shadowCoeff;
}

vec4 convertOutput(vec3 composition) {
    float compositionLength = length(composition);
    if(compositionLength < 1.0) {
        return vec4(composition, 0.0);
    }
    return vec4(
        normalize(composition),
        log2(compositionLength) / 3.0 // 3 because 2^3 = 8, its the max length
    );
}

void main(){

    const vec3 fireEffet = vec3(8.0, 0.2, 0.0);
    vec3 composition = ambientLightIntensity * mix(fs_vertexColor, fireEffet, burningCoeff);

    vec3 normalizedNormal = normalize(fs_vertexNormal);
    vec3 toCamera = normalize(cameraPosition - fs_vertexPositionWorld);
    if(dot(normalizedNormal, lightDirection) < 0.0) {
        float firstShadowCoeff = evaluateShadow(fs_vertexDepthMapSpace, depthTexture);
        if (firstShadowCoeff > 0.0) {
            composition += firstShadowCoeff * getLightContribution(
                normalizedNormal,
                toCamera,
                lightDirection,
                diffuseLightIntensity,
                specularLightIntensity
            );
        }
    }

    if(dot(normalizedNormal, light2Direction) < 0.0) {
        float secondShadowCoeff = evaluateShadow(fs_vertexDepthMap2Space, depth2Texture);
        if (secondShadowCoeff > 0.0) {
            composition += secondShadowCoeff * getLightContribution(
                normalizedNormal,
                toCamera,
                light2Direction,
                diffuseLight2Intensity,
                specularLight2Intensity
            );
        }
    }

    pixelColor = convertOutput(composition);
}

