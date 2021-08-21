#version 330 core

uniform mat4 VP;
uniform mat4 VPStar;

uniform float creationTime;
uniform float obtainingTime;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in float vs_itemDirection;
layout(location = 4) in vec3 vs_itemPosition;

out vec3 fs_vertexColor;
out vec4 fs_vertexDepthMapSpace;
out vec3 fs_vertexNormal;
out vec3 fs_vertexPositionWorld;

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

int intDir = int(vs_itemDirection);

bool itemIsGotten() {
    return (obtainingTime > 0);
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
    switch (intDir) {
        case 0:
        return rotationZ(-angle);
        case 1:
        return rotationZ(angle);
        case 2:
        return rotationX(angle);
        case 3:
        return rotationX(-angle);
        case 4:
        return rotationY(angle);
        case 5:
        return rotationY(-angle);
        default:
        return rotationY(angle);
    }
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
    switch (intDir) {
        case 0:
        return vec3(0.0, 0.0, -1.0);
        case 1:
        return vec3(0.0, 0.0, 1.0);
        case 2:
        return vec3(1.0, 0.0, 0.0);
        case 3:
        return vec3(-1.0, 0.0, 0.0);
        case 4:
        return vec3(0.0, 1.0, 0.0);
        case 5:
        return vec3(0.0, -1.0, 0.0);
        default:
        return vec3(0.0, 1.0, 0.0);
    }
}

mat4 itemTranslation() {
    if (!itemIsGotten()) {
        return mat4(1.0);
    }
    float translateCoeff = 0.0;
    if (obtainingTime < thresholdSecondStep) {
        translateCoeff = obtainingTime / thresholdSecondStep;
    }
    return translate(dirToVec());
}


void main() {

    mat4 translationToItem = translate(vs_itemPosition);

    mat4 rotation = itemRotation();
    mat4 translation = itemTranslation();
    mat4 scale = itemScale();

    mat4 modelTransform = translationToItem * translation * rotation * scale;

    mat4 normalTransform = rotation; // TODO: apply scale on normal

    const float w = 1.0;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    fs_vertexColor = vs_vertexColor;
    fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, w)).xyz);
    fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
    fs_vertexDepthMapSpace = VPStar * vertexPositionWorldSpace;

    gl_Position = VP * vertexPositionWorldSpace;
}
