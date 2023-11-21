uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 light2Direction;
    vec3 flashColor;
    float teleportationCoeff;
    vec3 ballPosition;
    vec3 cameraLookingDirection;
};

out vec4 pixelColor;
in vec2 fs_vertexPosition;
in vec3 fs_color;

void main() {
    const vec4 colorInside = vec4(1.0, 1.0, 1.0, 1.0);
    const vec3 backgroundColor = vec3(0.0, 0.0, 0.1);
    const float radiusInside = 0.6f;
    const float radiusOutside = 1.f;

    vec4 colorOutside = vec4(fs_color, 0.0);
    float dist = length(fs_vertexPosition);
    float coefficient = smoothstep(radiusInside, radiusOutside, dist);

    vec4 starRGBA = mix(colorInside, colorOutside, coefficient);
    float starAlpha = starRGBA.a;

    pixelColor = vec4(starRGBA.rgb * starAlpha + backgroundColor * (1.0 - starAlpha), 0.0);
}
