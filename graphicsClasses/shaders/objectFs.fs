#version 330 core

uniform vec4 arrowColor;
uniform bool isActivated;
out vec4 pixelColor;
in vec3 fs_vertexColor;
in vec3 fs_vertexNormal;

void main() {

    vec3 ambientColor = isActivated
                        ? fs_vertexColor
                        : vec3(0.2f, 0.2f, 0.2f);
    const vec3 toLight      = normalize(vec3(1.0, 1.0, 1.0));
    const vec3 toCamera     = vec3(0.0, 0.0, 1.0);
    vec3 reflection         = normalize(-toLight + 2.f * (dot(toLight, fs_vertexNormal)) * fs_vertexNormal);
    vec3 diffuseComponent   = vec3(max(0.f, dot(toLight, fs_vertexNormal)));
    vec3 specularComponent  = vec3(pow(max(0.f, dot(reflection, toCamera)), 20.f));
    vec3 composition        = ambientColor + diffuseComponent + specularComponent;
    pixelColor = vec4(composition, 1.f);
}
