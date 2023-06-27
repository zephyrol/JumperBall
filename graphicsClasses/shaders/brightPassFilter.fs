uniform sampler2D sceneTexture;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

const mat3 XYZToRGB = mat3(0.41847, -0.15866, -0.082835,
                           -0.091169, 0.25243, 0.015708,
                           0.00092090, -0.0025498, 0.17860);

vec3 convertCIExyYToRGB (vec3 CIExyYColor) {
    float scalar = CIExyYColor.z / CIExyYColor.y;
    vec3 CIEXYZ = vec3(scalar * CIExyYColor.x, CIExyYColor.z,
                       scalar * (1.0 - CIExyYColor.x - CIExyYColor.y));
    return XYZToRGB * CIEXYZ;
}

void main() {
    const float threshold = 4.0;
    // vec3 colorRGB = texture(textureScene, fs_vertexUVs).xyz;
    vec3 colorxyY = texture(sceneTexture, fs_vertexUVs).xyz;
    // Check if luminance is greater than threshold
    if (colorxyY.z > threshold) {
        vec3 colorRGB = convertCIExyYToRGB(colorxyY);
        pixelColor = vec4(colorRGB, 1.0);
    } else {
        pixelColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

}
