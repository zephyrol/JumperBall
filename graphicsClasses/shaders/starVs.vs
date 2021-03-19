#version 330 core

uniform mat4 VP;
uniform float distance;
uniform float radius;
uniform float creationTime;
uniform vec3 rotationCenter;

layout(location = 0) in vec3 vs_vertexPosition;

out vec2 fs_vertexPosition;

void main() {

    fs_vertexPosition = vs_vertexPosition.xy;

    const float w = 1.f;
    const vec3 rotationAxis = normalize(vec3(0.5, 1.0, 0.0));
    vec3 translationVector = vec3(0.0, 0.0, -distance);

    vec3 vertexPositionScaled = radius * vs_vertexPosition;
    vec3 translatedPos = vertexPositionScaled + translationVector;
    vec3 toPosition = normalize(translatedPos);

    vec3 vDotRotAxisxA = dot(toPosition, rotationAxis) * rotationAxis;

    const float radiansPerSeconds = 0.6;
    float angle = creationTime * radiansPerSeconds;
    vec3 toPositionAfterRotation = vDotRotAxisxA + (toPosition - vDotRotAxisxA) * cos(angle)
                                   + cross(rotationAxis, toPosition) * sin(angle);

    gl_Position = VP * vec4(length(translatedPos) * toPositionAfterRotation + rotationCenter, w);
}
