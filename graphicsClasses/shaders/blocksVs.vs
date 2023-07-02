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

uniform vec2 rotation[idCount];
uniform vec3 scale[idCount];
uniform vec3 translation[idCount];

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in vec3 vs_blockPosition;
layout(location = 4) in int vs_id;

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

mat4 rotationX (float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cosAngle, sinAngle, 0.0,
        0.0, -sinAngle, cosAngle, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
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

void main() {

    vec4 positionVec4 = vec4(scale[vs_id] * (vs_vertexPosition - vs_blockPosition), 1.0);
    vec2 rotationValue = rotation[vs_id];
    if(rotationValue.x == 1.0) {
        positionVec4 = rotationX(rotationValue.y) * positionVec4;
    } else if(rotationValue.x == 2.0) {
        positionVec4 = rotationY(rotationValue.y) * positionVec4;
    } else if(rotationValue.x == 3.0){
        positionVec4 = rotationZ(rotationValue.y) * positionVec4;
    }

    positionVec4 = vec4(positionVec4.xyz + vs_blockPosition + translation[vs_id], 1.f);

    #ifdef(LEVEL_PASS)
        fs_vertexColor = vs_vertexColor;
        fs_vertexNormal = vs_vertexNormal;
        fs_vertexPositionWorld = vec3(positionVec4);
        fs_vertexDepthMapSpace = biasMatrix * VPStar * positionVec4;
        fs_vertexDepthMap2Space = biasMatrix * VPStar2 * positionVec4;
        gl_Position = VP * positionVec4;
    #endif

    #ifdef(SHADOW_PASS)
        gl_Position = VPStar * positionVec4;
    #endif

    #ifdef(SHADOW_PASS_2)
        gl_Position = VPStar2 * positionVec4;
    #endif

}
