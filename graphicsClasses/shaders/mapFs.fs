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

uniform sampler2DShadow depthTexture;
uniform sampler2DShadow depth2Texture;
uniform sampler2D depthKernelTexture;

uniform vec4 shadowOffsets[4];

uniform int passId;

in vec3 fs_vertexColor;
in vec4 fs_vertexDepthMapSpace;
in vec4 fs_vertexDepthMap2Space;
in vec3 fs_vertexNormal;
in vec3 fs_vertexPositionWorld;

out vec4 pixelColor;

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
    if (dotReflectionToCamera < 0.0) {
        return diffuseComponent;
    }
    vec3 specularComponent = specularLightIntensity * pow(
        dotReflectionToCamera,
        20.0
    );
    return diffuseComponent + specularComponent;
}

float evaluateShadow(vec4 vertexDepthMapSpace, sampler2DShadow depthT) {
    vec4 kernelValue = texture(depthKernelTexture, (gl_FragCoord.xy * 0.5 + 0.5) * fragCoordToKernelUv);
    // vec4 kernelValue = texture(depthKernelTexture, vec2(0.0, 0.0));
    // vec4 kernelValue = vec4(0.0);
    vec4 offset = (kernelValue * 2.0 - 1.0) * shadowPixelsSize * 2.0;
    float shadowCoeff = textureProj(depthT, vertexDepthMapSpace + vec4(offset.xy, 0.0, 0.0));
    shadowCoeff += textureProj(depthT, vertexDepthMapSpace + vec4(offset.zw, 0.0, 0.0));
    // float shadowCoeff = textureProj(depthT, vertexDepthMapSpace + shadowOffsets[0]);
    // shadowCoeff += textureProj(depthT, vertexDepthMapSpace + shadowOffsets[2]);

    return shadowCoeff * 0.5;
}

vec4 convertOutput(vec3 composition) {
    float compositionLength = length(composition);
    if (compositionLength < 1.0) {
        return vec4(composition, 0.0);
    }
    return vec4(
        normalize(composition),
        (compositionLength - 1.0) / 7.0 // 7.0 because the max length is 8 (1 + 7)
    );
}


void main(){

    if(passId < 2){
        return;
    }

    vec3 composition = fs_vertexColor;

    vec3 normalizedNormal = normalize(fs_vertexNormal);
    vec3 toCamera = normalize(cameraPosition - fs_vertexPositionWorld);
    float firstShadowCoeff = evaluateShadow(fs_vertexDepthMapSpace, depthTexture);
    if (dot(normalizedNormal, lightDirection) < 0.0) {
        if (firstShadowCoeff > 0.9) {
            pixelColor = vec4(1.0, 1.0, 1.0, 0.0);
            return;
        }
        if (firstShadowCoeff > 0.4) {
            pixelColor = vec4(0.0, 0.0, 1.0, 0.0);
            return;
        }
        if (firstShadowCoeff > -0.1) {
            pixelColor = vec4(0.0, 0.0, 0.0, 0.0);
            return;
        }
    }
    pixelColor = vec4(0.0, 0.0, 0.0, 0.0);
}

