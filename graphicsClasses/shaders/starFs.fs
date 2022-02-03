#version 330 core

uniform Scene {
    mat4 VP;
    mat4 VPStar;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 flashColor;
    float teleportationCoeff;
};

out vec4 pixelColor;
in vec2 fs_vertexPosition;

void main() {
    const vec4 colorInside = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 colorOutside = vec4(0.0, 1.0, 1.0, 0.0);
    const float radiusInside = 0.3f;
    const float radiusOutside = 0.5f;

    // Star center at the position 0.f,0.f
    float dx = fs_vertexPosition.x;
    float dy = fs_vertexPosition.y;

    float dist = sqrt(dx * dx + dy * dy);
    float coefficient = smoothstep(radiusInside, radiusOutside, dist);

    pixelColor = mix(colorInside, colorOutside, coefficient);
}
