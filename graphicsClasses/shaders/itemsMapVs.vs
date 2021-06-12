#version 330 core

uniform mat4 VP;
uniform mat4 VPStar;

uniform float creationTime;
uniform float obtainingTime;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in float vs_objectDirection;
layout(location = 4) in vec3 vs_objectPosition;

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
mat4 upToNorth = rotationX(-PI_2);
mat4 upToSouth = rotationX(PI_2);
mat4 upToEast = rotationZ(-PI_2);
mat4 upToWest = rotationZ(PI_2);
mat4 upToUp = mat4(1.0);
mat4 upToDown = rotationX(PI);

mat4 rotationUpToDir (float direction) {
    int intDir = int(direction);
    switch (intDir) {
    case 0:
        return upToNorth;
    case 1:
        return upToSouth;
    case 2:
        return upToEast;
    case 3:
        return upToWest;
    case 4:
        return upToUp;
    case 5:
        return upToDown;
    default:
        return upToUp;
    }
    return upToUp;
}


const float thresholdSecondStep = 1.0;
const float thresholdThirdStep = 1.5;
const float durationSecondStep = thresholdThirdStep - thresholdSecondStep;
const float durationThirdStep = 0.2;

bool objectIsGotten() {
    return (obtainingTime > 0);
}

mat4 objectScale() {
    if (!objectIsGotten()) {
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

mat4 objectRotation() {
    if (!objectIsGotten()) {
        const float speedFactor = 5.0;
        return rotationY(speedFactor * creationTime);
    }
    const float speedPow = 5.0;
    return rotationY(pow(obtainingTime, speedPow));
}

mat4 objectTranslation() {
    if (!objectIsGotten()) {
        return mat4(1.0);
    }
    vec3 translateVector = vec3(0.0, 1.0, 0.0);
    if (obtainingTime < thresholdSecondStep) {
        translateVector = vec3(0.0, obtainingTime / thresholdSecondStep, 0.0);
    }
    return translate(translateVector);
}

void main() {

    mat4 translationOnBlock = translate(vec3(0.0, 0.75, 0.0));

    mat4 translationToBlock = translate(vs_objectPosition);
    mat4 initialRotation = rotationUpToDir(vs_objectDirection);

    mat4 rotation = objectRotation();
    mat4 translation = objectTranslation();
    mat4 scale = objectScale();
    mat4 modelTransform = translationToBlock * initialRotation * translationOnBlock *
                          translation * rotation * scale;
    mat4 normalTransform = initialRotation * rotation; // TODO: apply scale on normal

    const float w = 1.0;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    fs_vertexColor = vs_vertexColor;
    fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, w)).xyz);
    fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
    fs_vertexDepthMapSpace = VPStar * vertexPositionWorldSpace;

    gl_Position = VP * vertexPositionWorldSpace;
}
