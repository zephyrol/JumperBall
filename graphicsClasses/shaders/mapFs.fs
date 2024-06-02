uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    mat4 VDepthPStar;
    mat4 VDepthPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 light2Direction;
    vec3 flashColor;
    float teleportationCoeff;
};

uniform sampler2DShadow depthTexture;
uniform sampler2DShadow depth2Texture;
uniform sampler2D depthKernelTexture;

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
    vec4 offset = (kernelValue * 2.0 - 1.0) * shadowPixelsSize;
    float shadowCoeff = textureProj(depthT, vertexDepthMapSpace + vec4(offset.xy, 0.0, 0.0));
    shadowCoeff += textureProj(depthT, vertexDepthMapSpace + vec4(offset.zw, 0.0, 0.0));
    return shadowCoeff;
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
    if (dot(normalizedNormal, lightDirection) < 0.0) {
        float firstShadowCoeff = evaluateShadow(fs_vertexDepthMapSpace, depthTexture);
        if (firstShadowCoeff > 0.0) {
            composition += firstShadowCoeff * 0.5 * getLightContribution(
                normalizedNormal,
                toCamera,
                lightDirection,
                vec3(0.20, 0.25, 0.25),
                vec3(0.0, 0.25, 0.25)
            );
        }
    }

    if (dot(normalizedNormal, light2Direction) < 0.0) {
        float secondShadowCoeff = evaluateShadow(fs_vertexDepthMap2Space, depth2Texture);
        if (secondShadowCoeff > 0.0) {
            composition += secondShadowCoeff * 0.5 * getLightContribution(
                normalizedNormal,
                toCamera,
                light2Direction,
                vec3(0.25, 0.20, 0.25),
                vec3(0.25, 0.0, 0.25)
            );
        }
    }

    pixelColor = convertOutput(composition);
}

