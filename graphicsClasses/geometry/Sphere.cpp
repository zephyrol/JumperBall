/*
 * File: Sphere.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 14:43
 */

#include "Sphere.h"

Sphere::Sphere(const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    GeometricShape(modelTransform, normalsTransform, {}, {}) {
}

Sphere::Sphere(const glm::vec3 &customColor,
               const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    GeometricShape(modelTransform, normalsTransform, {customColor}, {}) {

}

Sphere::Sphere(const glm::vec3 &customColor,
               const glm::vec3 &customColor2,
               const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    GeometricShape(modelTransform, normalsTransform, {customColor, customColor2}, {}) {

}

Sphere::Sphere(const JBTypes::Color &color,
               const glm::mat4 &modelTransform,
               const glm::mat4 &normalsTransform) :
    Sphere(getSphereColor(color), modelTransform, normalsTransform) {

}

GeometricShape::ShapeVerticesInfo Sphere::computeBasicInfoSphere(
    bool useCustomColors,
    const glm::vec3 &firstColor,
    const glm::vec3 &secondColor
) {

    GeometricShape::ShapeVertexAttributes infoAttributesSphere;
    GeometricShape::IndicesBuffer indices;

    constexpr unsigned int iParaCount = 40;
    constexpr unsigned int iMeriCount = 60;
    constexpr float r = 1.f;

    // Create a sphere
    constexpr GLuint iVertexCount = iParaCount * iMeriCount;

    constexpr float a1 = (180.0f / static_cast <float>(iParaCount - 1)) * static_cast <float>(M_PI) / 180.0f;
    constexpr float a2 = (360.0f / (iMeriCount - 1)) * static_cast <float>(M_PI) / 180.0f;

    // Parallels
    for (unsigned int i = 0; i < iParaCount; ++i) {
        const auto iFloat = static_cast<float>(i);
        const float fAngle = -static_cast <float>(M_PI) / 2.0f + a1 * iFloat;
        const float z = r * sinf(fAngle);
        const float fRadius = r * cosf(fAngle);

        for (unsigned int j = 0; j < iMeriCount; ++j) {
            const auto jFloat = static_cast<float>(j);
            infoAttributesSphere.positions.emplace_back(
                fRadius * cosf(a2 * jFloat),
                fRadius * sinf(a2 * jFloat),
                z
            );
            infoAttributesSphere.uvCoords.emplace_back(
                jFloat / iMeriCount,
                static_cast <float>(iParaCount - i) / iParaCount
            );
            if (!useCustomColors) {
                infoAttributesSphere.colors.emplace_back(
                    iFloat / iParaCount,
                    (j < iMeriCount / 2) ? 1.f : 0.f,
                    0.5f
                );
            } else {
                infoAttributesSphere.colors.push_back((j < iMeriCount / 2) ? firstColor : secondColor);
            }
        }
    }
    // compute normals ---------------------------------------------------------
    // on a 0 centered sphere : you just need to normalise the position!
    infoAttributesSphere.normals.reserve(iVertexCount);

    for (unsigned int i = 0; i < iVertexCount; ++i) {
        infoAttributesSphere.normals.push_back(glm::normalize(infoAttributesSphere.positions[i]));
    }

    // for quads split in 2

    for (unsigned int i = 0; i < (iParaCount - 1); ++i) {
        for (unsigned int j = 0; j < (iMeriCount - 1); ++j) {
            indices.push_back(iMeriCount * i + j);
            indices.push_back(iMeriCount * i + (j + 1));
            indices.push_back(iMeriCount * (i + 1) + (j + 1));
            indices.push_back(iMeriCount * (i + 1) + (j + 1));
            indices.push_back(iMeriCount * (i + 1) + j);
            indices.push_back(iMeriCount * i + j);
        }
    }
    GeometricShape::ShapeVerticesInfo verticesInfo;
    verticesInfo.shapeVertexAttributes = infoAttributesSphere;
    verticesInfo.indices = indices;
    return verticesInfo;
}


std::vector<glm::vec3> Sphere::genColors(const std::vector<glm::vec3> &colors) const {
    if (colors.size() == 2) {
        return computeBasicInfoSphere(true, colors.at(0), colors.at(1)).shapeVertexAttributes.colors;
    }
    if (colors.size() == 1) {
        return GeometricShape::createCustomColorBuffer(
            colors.at(0), // customColor
            basicInfoSphere.shapeVertexAttributes.colors.size()
        );
    }
    return basicInfoSphere.shapeVertexAttributes.colors;
}

std::vector<GLushort> Sphere::genIndices() const {
    return basicInfoSphere.indices;
}

std::vector<glm::vec3> Sphere::genNormals() const {
    return basicInfoSphere.shapeVertexAttributes.normals;
}

std::vector<glm::vec3> Sphere::genPositions() const {
    return basicInfoSphere.shapeVertexAttributes.positions;
}


// TODO: create a dictionnary to don't keep this geometry and do not compute the geometry each time
const GeometricShape::ShapeVerticesInfo Sphere::basicInfoSphere = computeBasicInfoSphere();

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

