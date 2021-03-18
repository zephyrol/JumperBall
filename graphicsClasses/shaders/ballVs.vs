#version 330 core

uniform mat4 VP;
uniform mat4 VPStar;

uniform float creationTime;
uniform float obtainingTime;

uniform float ballRadius;
uniform float crushingCoeff;
uniform float status;
uniform float timeStateOfLife;
uniform vec3 sideDir;
uniform vec3 position;
uniform vec4 quaternion;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 4) in float vs_objectDirection;
layout(location = 5) in vec3 vs_objectPosition;

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

mat4 scaleMat (vec3 scale) {
    return mat4(scale.x, 0.0, 0.0, 0.0,
                0.0, scale.y, 0.0, 0.0,
                0.0, 0.0, scale.z, 0.0,
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

const float minScaleCrushing = 0.8;
float crushingScale = crushingCoeff * minScaleCrushing + (1.0 - crushingCoeff);

mat4 burstingScaleMatrix() {
    const float durationBursting = 0.07;
    const float radiusScalarBursting = 2.5;
    float t = timeStateOfLife / durationBursting;

    float scaleBursting;
    if (t > 1.0) {
        scaleBursting = 0.0;
    } else {
        scaleBursting = (1.0 - t) + radiusScalarBursting * t;
    }
    return scaleMat(vec3(scaleBursting * ballRadius));
}

mat4 ballScale() {
    if (status == 1.0) {
        return burstingScaleMatrix();
    }
    if (status == 2.0) {
        return mat4(0.0);
    }
    vec3 deformationVector = abs(sideDir);
    vec3 scaleVector = vec3(1.0) - (1.0 - crushingScale) * deformationVector;
    return scaleMat(scaleVector * ballRadius);
}

mat4 ballTranslation() {

    mat4 translation = translate(position);
    if (status != 0.0) {
        return translation;
    }
    vec3 translationVector = (crushingScale - 1.0) * ballRadius * sideDir;
    return translation * mat4(translate(translationVector));
}


mat4 ballRotation() {

    float x2 = quaternion.x * quaternion.x;
    float y2 = quaternion.y * quaternion.y;
    float z2 = quaternion.z * quaternion.z;
    float xy = quaternion.x * quaternion.y;
    float xz = quaternion.x * quaternion.z;
    float yz = quaternion.y * quaternion.z;
    float wx = quaternion.w * quaternion.x;
    float wy = quaternion.w * quaternion.y;
    float wz = quaternion.w * quaternion.z;
    return mat4(
        1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy), 0.0,
        2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx), 0.0,
        2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2), 0.0,
        0.0, 0.0, 0.0, 1.0
        );
}

void main() {

    mat4 rotation = ballRotation();
    mat4 translation = ballTranslation();
    mat4 scale = ballScale();
    mat4 modelTransform = translation * scale * rotation;
    mat4 normalTransform = rotation; // TODO: apply scale on normal

    const float w = 1.f;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    fs_vertexColor = vs_vertexColor;
    fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, w)).xyz);
    fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
    fs_vertexDepthMapSpace = VPStar * vertexPositionWorldSpace;

    gl_Position = VP * vertexPositionWorldSpace;
}
