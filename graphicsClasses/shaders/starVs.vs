#version 330 core

uniform Scene {
    mat4 VP;
    mat4 VPStar;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

// TODO Use vertex attributes (2 stars) or const variables (1 star)
uniform float starDistance;
uniform float radius;

uniform vec3 initialDirection;
uniform vec3 rotationCenter;

uniform vec4 rotationQuaternion;

layout(location = 0) in vec3 vs_vertexPosition;

out vec2 fs_vertexPosition;

mat4 starTranslation() {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                rotationCenter.x, rotationCenter.y, rotationCenter.z, 1.0);
}


mat4 starRotation() {

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

    fs_vertexPosition = vs_vertexPosition.xy;
    gl_Position = VP * starTranslation() * starRotation() *
                  vec4(starDistance * initialDirection + radius * vs_vertexPosition, 1.0);
}
