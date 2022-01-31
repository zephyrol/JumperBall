#version 330 core

layout (std140) uniform Scene {
    mat4 VP;
    mat4 VPStar;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

// out float depthValue;
out vec4 color;

void main() {
    // depthValue = gl_FragCoord.z;
    color = vec4(gl_FragCoord.z, 0.f, 0.f, 1.f);
}
