#version 330 core

//out float depthValue;
out float color;

void main() {
//depthValue = gl_FragCoord.z;
color = gl_FragCoord.z;
}
