#version 330 core

uniform mat4 VP;
uniform mat4 VPStar;

uniform vec3 translation;
uniform vec3 scale;

layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 2) in vec3 vs_vertexNormal;

out vec3 fs_vertexColor;
out vec4 fs_vertexDepthMapSpace;
out vec3 fs_vertexNormal;
out vec3 fs_vertexPositionWorld;

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
    mat4 normalTransform = mat4(1.0); // TODO: apply scale on normal

    const float w = 1.0;
    vec4 vertexPositionWorldSpace = modelTransform * vec4(vs_vertexPosition, w);

    fs_vertexColor = vs_vertexColor;
    fs_vertexNormal = normalize((normalTransform * vec4(vs_vertexNormal, w)).xyz);
    fs_vertexPositionWorld = vertexPositionWorldSpace.xyz;
    fs_vertexDepthMapSpace = VPStar * vertexPositionWorldSpace;

    gl_Position = VP * vertexPositionWorldSpace;
}
