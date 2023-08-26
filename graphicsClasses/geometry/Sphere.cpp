/*
 * File: Sphere.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 14:43
 */

#include "Sphere.h"

Sphere::Sphere(const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    GeometricShape(modelTransform, normalsTransform, {glm::vec3(0.f, 0.f, 0.f)}, {}),
    _isHq(false){
}

Sphere::Sphere(const glm::vec3 &customColor,
               const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    GeometricShape(modelTransform, normalsTransform, {customColor}, {}),
    _isHq(false)
    {

}

Sphere::Sphere(const glm::vec3 &customColor,
               const glm::vec3 &customColor2,
               const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    GeometricShape(modelTransform, normalsTransform, {customColor, customColor2}, {}),
    _isHq(true){

}

Sphere::Sphere(const JBTypes::Color &color,
               const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    Sphere(getSphereColor(color), modelTransform, normalsTransform)
{
}

Sphere::Sphere(
    std::vector<glm::vec3> &&customColors,
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform
) : GeometricShape(modelTransform, normalsTransform, std::move(customColors), {}),
_isHq(true){
}


std::vector<glm::vec3> Sphere::genColors(const std::vector<glm::vec3> &colors) const {
    const auto iParaCount = _isHq ? hqIParaCount : sdIParaCount;
    const auto iMeriCount = _isHq ? hqIMeriCount : sdIMeriCount;
    if (colors.size() == 1) {
        const size_t numberOfColors = iParaCount* iMeriCount;
        return GeometricShape::createCustomColorBuffer(
            colors.at(0), // customColor
            numberOfColors
        );
    }

    std::vector<glm::vec3> outputColors{};
    // Parallels
    for (unsigned int i = 0; i < iParaCount; ++i) {
        for (unsigned int j = 0; j < iMeriCount; ++j) {
            const size_t colorNumber = j * colors.size() / iMeriCount;
            outputColors.emplace_back(colors.at(colorNumber));
        }
    }
    return outputColors;
}

std::vector<GLushort> Sphere::genIndices() const {

    std::vector<GLushort> indices{};
    const auto iParaCount = _isHq ? hqIParaCount : sdIParaCount;
    const auto iMeriCount = _isHq ? hqIMeriCount : sdIMeriCount;

    // For quads split in 2
    for (unsigned int i = 0; i < (iParaCount- 1); ++i) {
        for (unsigned int j = 0; j < (iMeriCount- 1); ++j) {
            indices.push_back(iMeriCount * i + j);
            indices.push_back(iMeriCount * i + (j + 1));
            indices.push_back(iMeriCount * (i + 1) + (j + 1));
            indices.push_back(iMeriCount * (i + 1) + (j + 1));
            indices.push_back(iMeriCount * (i + 1) + j);
            indices.push_back(iMeriCount * i + j);
        }
    }
    return indices;
}

std::vector<glm::vec3> Sphere::genNormals() const {
    std::vector<glm::vec3> normals{};

    const auto iParaCount = _isHq ? hqIParaCount : sdIParaCount;
    const auto iMeriCount = _isHq ? hqIMeriCount : sdIMeriCount;
    // Create a sphere
    const GLuint iVertexCount = iParaCount * iMeriCount;

    // Compute normals
    // on a 0 centered sphere : you just need to normalise the position!
    normals.reserve(iVertexCount);

    const std::vector<glm::vec3> positions = genPositions();
    for (unsigned int i = 0; i < iVertexCount; ++i) {
        normals.emplace_back(glm::normalize(positions[i]));
    }
    return normals;
}

std::vector<glm::vec3> Sphere::genPositions() const {
    std::vector<glm::vec3> positions{};
    constexpr float r = 1.f;

    const auto iParaCount = _isHq ? hqIParaCount : sdIParaCount;
    const auto iMeriCount = _isHq ? hqIMeriCount : sdIMeriCount;

    const float a1 = (180.0f / static_cast <float>(iParaCount - 1)) * JBTypes::pi / 180.0f;
    const float a2 = (360.0f / static_cast<float>(iMeriCount - 1)) * JBTypes::pi / 180.0f;

    // Parallels
    for (unsigned int i = 0; i < iParaCount; ++i) {
        const auto iFloat = static_cast<float>(i);
        const float fAngle = - JBTypes::pi / 2.0f + a1 * iFloat;
        const float z = r * sinf(fAngle);
        const float fRadius = r * cosf(fAngle);

        for (unsigned int j = 0; j < iMeriCount; ++j) {
            const auto jFloat = static_cast<float>(j);
            positions.emplace_back(
                fRadius * cosf(a2 * jFloat),
                fRadius * sinf(a2 * jFloat),
                z
            );
        }
    }
    return positions;
}

glm::vec3 Sphere::getSphereColor(const JBTypes::Color &color) {
    if (color == JBTypes::Color::Yellow) {
        return {1.f, 215.f / 255.f, 0.f};
    }
    if (color == JBTypes::Color::Red) {
        return {114.f / 255.f, 47.f / 255.f, 55.f / 255.f};
    }
    if (color == JBTypes::Color::Blue) {
        return {0.f, 0.f, 0.5f};
    }
    return {0.f, 0.f, 0.f};
}

