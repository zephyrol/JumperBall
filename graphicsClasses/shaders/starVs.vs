uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 light2Direction;
    vec3 flashColor;
    float teleportationCoeff;
    vec3 ballPosition;
    vec3 ballUp;
    vec3 ballLook;
    vec3 upBorder;
    vec3 frontBorder;
    vec3 rearBorder;
};

uniform float timeSinceCreation[idCount];

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_centerToStar;
layout(location = 2) in vec3 vs_rotationCenter;
layout(location = 3) in vec3 vs_rotationAxis;
layout(location = 4) in vec3 vs_color;
// Location 5 is uvs ...
layout(location = 6) in float vs_velocity;
layout(location = 7) in int vs_id;

out vec2 fs_vertexPosition;
out vec3 fs_color;

vec4 createRotationQuaternion() {
    float angle = vs_velocity * timeSinceCreation[vs_id];
    float angleOverTwo = angle / 2.0;
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

    const float starRadius = 50.0;

    fs_vertexPosition = vs_vertexPosition.xy;
    fs_color = vs_color;
    vec4 projectedPosition = VP * starTranslation() * starRotation() * vec4(
        vs_centerToStar + starRadius * (vs_vertexPosition * 0.5), 1.0 // * 0.5 to apply a scale and save fs.
    );

    // Z is cropped to 0 to display the star in all cases
    // Z Buffer must be disabled before!
    gl_Position = vec4(projectedPosition.x, projectedPosition.y, 0.0, projectedPosition.w);
}
