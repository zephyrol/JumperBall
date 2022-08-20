uniform Menu {
    float positionY;
};

//
layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;

out vec3 fs_vertexColor;

void main() {
    fs_vertexColor = vs_vertexColor;
    gl_Position = vec4(vs_vertexPosition.xy + vec2(0.0, positionY * 2.0), 0.0, 1.0);
}
