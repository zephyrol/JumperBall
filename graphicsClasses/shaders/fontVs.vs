layout(location = 0) in vec3 vs_vertexPosition;
layout(location = 1) in vec3 vs_vertexColor;
layout(location = 3) in vec2 vs_vertexUVs;
layout(location = 4) in vec2 vs_labelType;

out vec2 fs_vertexUVs;
out vec3 fs_vertexColor;
out vec2 fs_labelType;

#ifdef(SCROLLABLE)
    uniform float positionY;
#endif


void main() {
    fs_vertexUVs = vs_vertexUVs;
    fs_vertexColor = vs_vertexColor;
    fs_labelType = vs_labelType;

    vec2 positionXY = vs_vertexPosition.xy;
    #ifdef(SCROLLABLE)
        positionXY += vec2(0.0, positionY);
    #endif
    gl_Position = vec4(positionXY, 0.0, 1.0);
}
