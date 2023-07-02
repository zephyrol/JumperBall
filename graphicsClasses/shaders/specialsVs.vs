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
uniform float isRedActivated[idCount];
uniform float isGreenActivated[idCount];
uniform float isBlueActivated[idCount];
uniform float isYellowActivated[idCount];

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in vec3 vs_specialPosition;
layout(location = 4) in int vs_id;
layout(location = 5) in int vs_specialDirection;
layout(location = 6) in int vs_specialColor;
layout(location = 7) in int vs_isAnimated;

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

mat4 scaleYMat (float scaleY) {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, scaleY, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

const float PI = 3.14159265358979323846;
const float PI_2 = 1.57079632679489661923;

mat4 getUpToNorth() {
    return rotationX(-PI_2);
}

mat4 getUpToSouth() {
    return rotationX(PI_2);
}

mat4 getUpToEast() {
    return rotationZ(-PI_2);
}

mat4 getUpToWest() {
    return rotationZ(PI_2);
}

mat4 getUpToUp() {
    return mat4(1.0);
}

mat4 getUpToDown() {
    return rotationX(PI);
}

mat4 rotationUpToDir () {
    // Cast to int does not work on Apple M1...
    if (vs_specialDirection == 0) {
        return getUpToNorth();
    }
    if (vs_specialDirection == 1) {
        return getUpToSouth();
    }
    if (vs_specialDirection == 2) {
        return getUpToEast();
    }
    if (vs_specialDirection == 3) {
        return getUpToWest();
    }
    if (vs_specialDirection == 4) {
        return getUpToUp();
    }
    if (vs_specialDirection == 5) {
        return getUpToDown();
    }
    return getUpToUp();
}

bool isColorActivated() {
    if (vs_specialColor == 1) {
        return isRedActivated[vs_id] == 1.0;
    }
    if (vs_specialColor == 2) {
        return isGreenActivated[vs_id] == 1.0;
    }
    if (vs_specialColor == 3) {
        return isBlueActivated[vs_id] == 1.0;
    }
    if (vs_specialColor == 4) {
        return isYellowActivated[vs_id] == 1.0;
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
    const float speedCoefficient = 5.0;
    float angle = creationTime[vs_id] * speedCoefficient;

    if (vs_isAnimated == 1 && isActivated) {
        return rotationY(angle);
    }
    return mat4(1.0);
}

void main() {

    const mat4 translationOnBlock = mat4(1.0, 0.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0, 0.0,
                                         0.0, 0.0, 1.0, 0.0,
                                         0.0, 0.5, 0.0, 1.0);

    mat4 translationToBlock = translate(vs_specialPosition);
    mat4 initialRotation = rotationUpToDir();

    bool isActivated = isColorActivated();

    mat4 rotation = specialRotation(isActivated);
    mat4 scale = specialScale(isActivated);
    mat4 modelTransform = translationToBlock * initialRotation * translationOnBlock * rotation * scale;

    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, 1.0);

    #ifdef(LEVEL_PASS)
        fs_vertexColor = vs_vertexColor;
        mat4 normalTransform = initialRotation * rotation; // TODO: apply scale on normal
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
