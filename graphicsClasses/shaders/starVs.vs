#version 300 es

uniform Scene {
    mat4 VP;
    mat4 VPStar;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

uniform float timeSinceCreation;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 4) in vec3 vs_centerToStar;
layout(location = 5) in vec3 vs_rotationCenter;
layout(location = 6) in vec3 vs_rotationAxis;

out vec2 fs_vertexPosition;

vec4 createRotationQuaternion() {
    const float radiansPerSeconds = 0.6;
    float angle = radiansPerSeconds * timeSinceCreation;
    float angleOverTwo = angle / 2;
    return vec4(vec3(sin(angleOverTwo) * vs_rotationAxis), cos(angleOverTwo));
}

mat4 starTranslation() {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                vs_rotationCenter.x, vs_rotationCenter.y, vs_rotationCenter.z, 1.0);
}


mat4 starRotation() {
    vec4 rotationQuaternion = createRotationQuaternion();
    float x2 = rotationQuaternion.x * rotationQuaternion.x;
    float y2 = rotationQuaternion.y * rotationQuaternion.y;
    float z2 = rotationQuaternion.z * rotationQuaternion.z;
    float xy = rotationQuaternion.x * rotationQuaternion.y;
    float xz = rotationQuaternion.x * rotationQuaternion.z;
    float yz = rotationQuaternion.y * rotationQuaternion.z;
    float wx = rotationQuaternion.w * rotationQuaternion.x;
    float wy = rotationQuaternion.w * rotationQuaternion.y;
    float wz = rotationQuaternion.w * rotationQuaternion.z;
    return mat4(
        1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy), 0.0,
        2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx), 0.0,
        2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2), 0.0,
        0.0, 0.0, 0.0, 1.0
        );
}


void main() {

    const float starRadius = 5.0;

    fs_vertexPosition = vs_vertexPosition.xy;

    gl_Position = VP * starTranslation() * starRotation() *
                  vec4(vs_centerToStar + starRadius * vs_vertexPosition, 1.0);
}
