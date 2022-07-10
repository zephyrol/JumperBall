uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

uniform vec3 scale;
uniform vec3 translation;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;
layout(location = 3) in vec3 vs_blockPosition;

#ifdef(LEVEL_PASS)
    out vec3 fs_vertexColor;
    out vec4 fs_vertexDepthMapSpace;
    out vec4 fs_vertexDepthMap2Space;
    out vec3 fs_vertexNormal;
    out vec3 fs_vertexPositionWorld;
#endif

void main() {
    vec3 position = (vs_vertexPosition - vs_blockPosition) * vec3(scale) + vs_blockPosition + translation;
    vec4 positionVec4 = vec4(position, 1.0);

    #ifdef(LEVEL_PASS)
        fs_vertexColor = vs_vertexColor;
        fs_vertexNormal = vs_vertexNormal; // normalize((N * vec4(vs_vertexNormal,1.0)).xyz);
        fs_vertexPositionWorld = position;
        fs_vertexDepthMapSpace = VPStar * positionVec4;
        fs_vertexDepthMap2Space = VPStar2 * positionVec4;
        gl_Position = VP * positionVec4;
    #endif

    #ifdef(SHADOW_PASS)
        gl_Position = VPStar * positionVec4;
    #endif

    #ifdef(SHADOW_PASS_2)
        gl_Position = VPStar2 * positionVec4;
    #endif

}
