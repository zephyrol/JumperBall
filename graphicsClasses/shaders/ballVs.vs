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

uniform float creationTime[idCount];
uniform float ballRadius[idCount];
uniform float crushingCoeff[idCount];
uniform float status[idCount];
uniform float timeStateOfLife[idCount];
uniform vec3 sideDir[idCount];
uniform vec3 position[idCount];
uniform vec4 quaternion[idCount];

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in vec3 vs_objectPosition;
layout(location = 4) in float vs_objectDirection;
layout(location = 5) in int vs_id;

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

mat4 scaleMat (vec3 scale) {
    return mat4(scale.x, 0.0, 0.0, 0.0,
                0.0, scale.y, 0.0, 0.0,
                0.0, 0.0, scale.z, 0.0,
                0.0, 0.0, 0.0, 1.0);
}


const float thresholdSecondStep = 1.0;
const float thresholdThirdStep = 1.5;
const float durationSecondStep = thresholdThirdStep - thresholdSecondStep;
const float durationThirdStep = 0.2;

const float minScaleCrushing = 0.8;

mat4 burstingScaleMatrix() {
    const float durationBursting = 0.07;
    const float radiusScalarBursting = 2.5;
    float t = timeStateOfLife[vs_id] / durationBursting;

    float scaleBursting;
    if (t > 1.0) {
        scaleBursting = 0.0;
    } else {
        scaleBursting = (1.0 - t) + radiusScalarBursting * t;
    }
    return scaleMat(vec3(scaleBursting * ballRadius[vs_id]));
}

mat4 ballScale(float crushingScale) {
    if (status[vs_id] == 1.0) {
        return burstingScaleMatrix();
    }
    if (status[vs_id] == 2.0) {
        return mat4(0.0);
    }
    vec3 deformationVector = abs(sideDir[vs_id]);
    vec3 scaleVector = vec3(1.0) - (1.0 - crushingScale) * deformationVector;
    return scaleMat(scaleVector * ballRadius[vs_id]);
}

mat4 ballTranslation(float crushingScale) {

    mat4 translation = translate(position[vs_id]);
    if (status[vs_id] != 0.0) {
        return translation;
    }
    vec3 translationVector = (crushingScale - 1.0) * ballRadius[vs_id] * sideDir[vs_id];
    return translation * mat4(translate(translationVector));
}


mat4 ballRotation() {
    vec4 ballQuaternion = quaternion[vs_id];
    float x2 = ballQuaternion.x * ballQuaternion.x;
    float y2 = ballQuaternion.y * ballQuaternion.y;
    float z2 = ballQuaternion.z * ballQuaternion.z;
    float xy = ballQuaternion.x * ballQuaternion.y;
    float xz = ballQuaternion.x * ballQuaternion.z;
    float yz = ballQuaternion.y * ballQuaternion.z;
    float wx = ballQuaternion.w * ballQuaternion.x;
    float wy = ballQuaternion.w * ballQuaternion.y;
    float wz = ballQuaternion.w * ballQuaternion.z;
    return mat4(
        1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy), 0.0,
        2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx), 0.0,
        2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2), 0.0,
        0.0, 0.0, 0.0, 1.0
        );
}


void main() {

    float crushingScale = crushingCoeff[vs_id] * minScaleCrushing + (1.0 - crushingCoeff[vs_id]);
    mat4 rotation = ballRotation();
    mat4 translation = ballTranslation(crushingScale);
    mat4 scale = ballScale(crushingScale);
    mat4 modelTransform = translation * scale * rotation;

    const float w = 1.0;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    #ifdef(LEVEL_PASS)
        fs_vertexColor = vs_vertexColor;
        mat4 normalTransform = rotation; // TODO: apply scale on normal
        fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, w)).xyz);
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
