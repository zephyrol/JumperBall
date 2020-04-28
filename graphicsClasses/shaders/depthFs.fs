#version 330 core

//out float depthValue;
out vec4 color;

void main() {
//depthValue = gl_FragCoord.z;
color = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1.f);
}
