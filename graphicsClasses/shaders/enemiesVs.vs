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
uniform vec3 scale[idCount];

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in int vs_id;

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

mat4 translate (vec3 translationVec) {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                translationVec.x, translationVec.y, translationVec.z, 1.0);
}

mat4 scaleMat (vec3 scaleVec) {
    return mat4(scaleVec.x, 0.0, 0.0, 0.0,
                0.0, scaleVec.y, 0.0, 0.0,
                0.0, 0.0, scaleVec.z, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

void main() {
    mat4 modelTransform = translate(translation[vs_id]) * scaleMat(scale[vs_id]);

    const float w = 1.0;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    #ifdef(LEVEL_PASS)
        fs_vertexColor = vs_vertexColor;
        fs_vertexNormal = vs_vertexNormal;
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
