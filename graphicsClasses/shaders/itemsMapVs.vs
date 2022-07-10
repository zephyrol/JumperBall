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

uniform float creationTime;
uniform float obtainingTime;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in float vs_itemDirection;
layout(location = 4) in vec3 vs_itemPosition;

#ifdef(LEVEL_PASS)
    out vec3 fs_vertexColor;
    out vec4 fs_vertexDepthMapSpace;
    out vec4 fs_vertexDepthMap2Space;
    out vec3 fs_vertexNormal;
    out vec3 fs_vertexPositionWorld;

    const mat4 biasMatrix = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );
#endif

mat4 translate (vec3 translation) {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                translation.x, translation.y, translation.z, 1.0);
}

mat4 rotationX (float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, cosAngle, sinAngle, 0.0,
                0.0, -sinAngle, cosAngle, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 rotationY (float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return mat4(cosAngle, 0.0, -sinAngle, 0.0,
                0.0, 1.0, 0.0, 0.0,
                sinAngle, 0.0, cosAngle, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 rotationZ (float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return mat4(cosAngle, sinAngle, 0.0, 0.0,
                -sinAngle, cosAngle, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 scaleMat (float scale) {
    return mat4(scale, 0.0, 0.0, 0.0,
                0.0, scale, 0.0, 0.0,
                0.0, 0.0, scale, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

const float PI = 3.14159265358979323846;
const float PI_2 = 1.57079632679489661923;

const float thresholdSecondStep = 1.0;
const float thresholdThirdStep = 1.5;
const float durationSecondStep = thresholdThirdStep - thresholdSecondStep;
const float durationThirdStep = 0.2;

bool itemIsGotten() {
    return (obtainingTime > 0.0);
}

mat4 itemScale() {
    if (!itemIsGotten()) {
        return mat4(1.0);
    }
    if (obtainingTime < thresholdSecondStep) {
        return mat4(1.0);
    }
    if (obtainingTime < thresholdThirdStep) {
        return scaleMat(1.0 + ((obtainingTime - thresholdSecondStep) / durationSecondStep));
    }
    if (obtainingTime < thresholdThirdStep + durationThirdStep) {
        return scaleMat(2.0 * (1.0 - ((obtainingTime - thresholdThirdStep) / durationThirdStep)));
    }
    return mat4(0.0);
}

mat4 computeRotationMatrix(float angle) {
    // Cast to int does not work on Apple M1...
    if (vs_itemDirection == 0.0) {
        return rotationZ(-angle);
    }
    if (vs_itemDirection == 1.0) {
        return rotationZ(angle);
    }
    if (vs_itemDirection == 2.0) {
        return rotationX(angle);
    }
    if (vs_itemDirection == 3.0) {
        return rotationX(-angle);
    }
    if (vs_itemDirection == 4.0) {
        return rotationY(angle);
    }
    if (vs_itemDirection == 5.0) {
        return rotationY(-angle);
    }
    return rotationY(angle);
}

mat4 itemRotation() {
    if (!itemIsGotten()) {
        const float speedFactor = 5.0;
        return computeRotationMatrix(speedFactor * creationTime);
    }
    const float speedPow = 5.0;
    return computeRotationMatrix(pow(obtainingTime, speedPow));
}

vec3 dirToVec() {
    if (vs_itemDirection == 0.0) {
        return vec3(0.0, 0.0, -1.0);
    }
    if (vs_itemDirection == 1.0) {
        return vec3(0.0, 0.0, 1.0);
    }
    if (vs_itemDirection == 2.0) {
        return vec3(1.0, 0.0, 0.0);
    }
    if (vs_itemDirection == 3.0) {
        return vec3(-1.0, 0.0, 0.0);
    }
    if (vs_itemDirection == 4.0) {
        return vec3(0.0, 1.0, 0.0);
    }
    if (vs_itemDirection == 5.0) {
        return vec3(0.0, -1.0, 0.0);
    }
    return vec3(0.0, 1.0, 0.0);
}

mat4 itemTranslation() {
    if (!itemIsGotten()) {
        return mat4(1.0);
    }
    float translateCoeff = 0.0;
    vec3 wayDir = 0.7 * dirToVec();
    if (obtainingTime < thresholdSecondStep) {
        translateCoeff = obtainingTime / thresholdSecondStep;
        return translate(translateCoeff * wayDir);
    }
    return translate(wayDir);
}


void main() {

    mat4 translationToItem = translate(vs_itemPosition);

    mat4 rotation = itemRotation();
    mat4 translation = itemTranslation();
    mat4 scale = itemScale();

    mat4 modelTransform = translationToItem * translation * rotation * scale;


    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, 1.0);

    #ifdef(LEVEL_PASS)
        fs_vertexColor = vs_vertexColor;
        mat4 normalTransform = rotation; // TODO: apply scale on normal
        fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, 1.0)).xyz);
        fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
        fs_vertexDepthMapSpace = biasMatrix * VPStar * vertexPositionWorldSpace;
        fs_vertexDepthMap2Space = biasMatrix * VPStar2 * vertexPositionWorldSpace;
        gl_Position = VP * vertexPositionWorldSpace;
    #endif

    #ifdef(SHADOW_PASS)
        gl_Position = VPStar * vertexPositionWorldSpace;
    #endif

    #ifdef(SHADOW_PASS_2)
        gl_Position = VPStar2 * vertexPositionWorldSpace;
    #endif

}
