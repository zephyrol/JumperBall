#version 330 core

uniform mat4 VP;
uniform mat4 VPStar;

uniform float creationTime;
uniform float isRedActivated;
uniform float isGreenActivated;
uniform float isBlueActivated;
uniform float isYellowActivated;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in float vs_specialDirection;
layout(location = 4) in float vs_specialColor;
layout(location = 5) in float vs_isAnimated;
layout(location = 6) in vec3 vs_specialPosition;

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

mat4 scaleYMat (float scaleY) {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, scaleY, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
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

bool isColorActivated() {
    int intColor = int(vs_specialColor);
    switch (intColor) {
    case 1:
        return isRedActivated == 1.0;
    case 2:
        return isGreenActivated == 1.0;
    case 3:
        return isBlueActivated == 1.0;
    case 4:
        return isYellowActivated == 1.0;
    }
    return false;
}

mat4 specialScale (bool isActivated) {
    int intAnimated = int(vs_isAnimated);
    if (intAnimated == 0 && isActivated) {
        return scaleYMat(0.1);
    }
    return mat4(1.0);
}

mat4 specialRotation (bool isActivated) {
    int intAnimated = int(vs_isAnimated);
    int intDirection = int(vs_specialDirection);
    const float speedCoefficient = 5.0;
    float angle = creationTime * speedCoefficient;

    if (intAnimated == 1 && isActivated) {
        return rotationY(angle);
    }
    return mat4(1.0);
}

/*mat4 specialTranslation(bool isActivated) {
    int intAnimated = int(vs_isAnimated);
    if (intAnimated == 0 && isActivated) {
        return translate(vec3());
    }
    return mat4(1.0);
   }*/

void main() {

    mat4 translationOnBlock = translate(vec3(0.0, 0.5, 0.0));

    mat4 translationToBlock = translate(vs_specialPosition);
    mat4 initialRotation = rotationUpToDir(vs_specialDirection);

    bool isActivated = isColorActivated();

    mat4 rotation = specialRotation(isActivated);
    // mat4 translation = specialTranslation(isActivated);
    mat4 scale = specialScale(isActivated);
    mat4 modelTransform = translationToBlock * initialRotation * translationOnBlock *
                          /*translation*/ rotation * scale;
    mat4 normalTransform = initialRotation * rotation; // TODO: apply scale on normal

    const float w = 1.0;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    fs_vertexColor = vs_vertexColor;
    fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, w)).xyz);
    fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
    fs_vertexDepthMapSpace = VPStar * vertexPositionWorldSpace;

    gl_Position = VP * vertexPositionWorldSpace;
}
