uniform Scene {
    mat4 VP;
    mat4 VPStar;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

uniform vec3 translation;
uniform vec3 scale;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;

#ifdef(LEVEL_PASS)
    out vec3 fs_vertexColor;
    out vec4 fs_vertexDepthMapSpace;
    out vec3 fs_vertexNormal;
    out vec3 fs_vertexPositionWorld;
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
    mat4 modelTransform = translate(translation) * scaleMat(scale);

    const float w = 1.0;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    #ifdef(LEVEL_PASS)
        fs_vertexColor = vs_vertexColor;

        // TODO: apply scale on normal
        // mat4 normalTransform = mat4(1.0);
        // fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, w)).xyz);
        fs_vertexNormal = vs_vertexNormal;
        fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
        fs_vertexDepthMapSpace = VPStar * vertexPositionWorldSpace;
        gl_Position = VP * vertexPositionWorldSpace;
    #endif

    #ifdef(SHADOW_PASS)
        gl_Position = VPStar * vertexPositionWorldSpace;
    #endif
}
