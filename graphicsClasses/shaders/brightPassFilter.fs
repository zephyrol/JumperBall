uniform sampler2D textureScene;

in vec2 fs_vertexUVs;
out vec4 pixelColor;

const mat3 RGBToXYZ = mat3(
    2.7689, 1.7517, 1.1302,
    1.0000, 4.5907, 0.060100,
    0.0000, 0.056508, 5.5943
);

vec3 convertRGBToCIExyY (vec3 rbgColor) {

    vec3 CIEXYZ = RGBToXYZ * rbgColor;
    float sumXYZ = CIEXYZ.x + CIEXYZ.y + CIEXYZ.z;
    return vec3(CIEXYZ.x / sumXYZ, CIEXYZ.y / sumXYZ, CIEXYZ.y);
}

vec3 convertInput(vec4 scenePixel) {
    if(scenePixel.a == 0.0) {
        return scenePixel.xyz;
    }
    float length = exp2(scenePixel.a * 3.0);
    return scenePixel.xyz * length;
}

void main() {
    const float threshold = 4.0;
    vec4 scenePixel = texture(textureScene, fs_vertexUVs);
    vec3 colorRGB = convertInput(scenePixel);
    vec3 colorxyY = convertRGBToCIExyY(colorRGB);
    // Check if luminance is greater than the threshold.
    if (colorxyY.z > threshold) {
        pixelColor = scenePixel;
    } else {
        pixelColor = vec4(0.0, 0.0, 0.0, 0.0);
    }

}
