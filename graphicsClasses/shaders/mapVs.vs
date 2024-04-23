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

uniform vec3 translation[idCount];
uniform vec4 rotation[idCount];
uniform vec3 scale[idCount];
uniform float burningCoeff[idCount];

uniform int passId;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in int vs_id;

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

mat4 getTranslationMatrix(vec3 translation) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        translation.x, translation.y, translation.z, 1.0
    );
}

mat4 getScaleMatrix(vec3 scale) {
    return mat4(
        scale.x, 0.0, 0.0, 0.0,
        0.0, scale.y, 0.0, 0.0,
        0.0, 0.0, scale.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 getRotationMatrix(vec4 quaternion) {
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

const vec3 fireEffect = vec3(8.0, 0.2, 0.0);
const vec3 ambientLightIntensity = vec3(0.7, 0.7, 0.7);

void main() {

    mat4 translationMatrix = getTranslationMatrix(translation[vs_id]);
    mat4 rotationMatrix = getRotationMatrix(rotation[vs_id]);
    mat4 scaleMatrix = getScaleMatrix(scale[vs_id]);
    mat4 modelTransform = translationMatrix * scaleMatrix * rotationMatrix;
    vec4 vertexPositionWorldSpace = modelTransform *  vec4(vs_vertexPosition, 1.0);

    if(passId == 0) {
        gl_Position = VPStar * vertexPositionWorldSpace;
        return;
    }

    if(passId == 1) {
        gl_Position = VPStar2 * vertexPositionWorldSpace;
        return;
    }
    fs_vertexColor = mix(vs_vertexColor, fireEffect, burningCoeff[vs_id]) * ambientLightIntensity;
    mat4 normalTransform = rotationMatrix;
    fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, 1.0)).xyz);
    fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
    fs_vertexDepthMapSpace = biasMatrix * VPStar * vertexPositionWorldSpace;
    fs_vertexDepthMap2Space = biasMatrix * VPStar2 * vertexPositionWorldSpace;
    gl_Position = VP * vertexPositionWorldSpace;

}
