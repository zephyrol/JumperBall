

uniform Scene {
    mat4 VP;
    mat4 VPStar;
    mat4 VPStar2;
    vec3 cameraPosition;
    vec3 lightDirection;
    vec3 light2Direction;
    vec3 flashColor;
    float teleportationCoeff;
};

// uniform float timeSinceCreation[idCount];

layout(location = 0) in vec3 vs_vertexPosition;

out vec2 fs_vertexPosition;
out vec3 fs_color;

const vec4 test = vec4(1.0);
vec4 createRotationQuaternion(vec4 a) {
    return test;
}
void main() {

    fs_vertexPosition = vs_vertexPosition.xy;
    fs_color = vec3(1.0);

    mat4 starTranslation = mat4(1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0);
    vec4 projectedPosition = starTranslation * createRotationQuaternion(vec4(2.0)) *  vec4(
        vec3(1.0) + 50.0 * (vs_vertexPosition * 0.5), 1.0 // * 0.5 to apply a scale and save fs.
    );

    // Z is cropped to 0 to display the star in all cases
    // Z Buffer must be disabled before!
    gl_Position = vec4(projectedPosition.x, projectedPosition.y, 0.0, projectedPosition.w);
}

