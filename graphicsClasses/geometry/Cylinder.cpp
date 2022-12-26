/*
 * File: Cylinder.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:19
 */

#include "Cylinder.h"

Cylinder::Cylinder(
    size_t meriCount,
    const glm::mat4& modelTransform,
    const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, {}, {}),
    _meriCount(meriCount) {
}

Cylinder::Cylinder(
    const glm::vec3& customColor,
    size_t meriCount,
    const glm::mat4& modelTransform,
    const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, { customColor }, {}),
    _meriCount(meriCount) {
}

Cylinder::Cylinder(const glm::vec3& customColorCenter,
                   const glm::vec3& customColorEdge,
                   size_t meriCount,
                   const glm::mat4& modelTransform,
                   const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, { customColorCenter, customColorEdge }, {}),
    _meriCount(meriCount) {
}

Cylinder::Cylinder(const JBTypes::Color &color,
                   size_t meriCount,
                   const glm::mat4 &modelTransform,
                   const glm::mat4 &normalsTransform):
    Cylinder(
        getCenterAndEdgeColor(color).at(0),
        getCenterAndEdgeColor(color).at(1),
        meriCount,
        modelTransform,
        normalsTransform
    ) {

}

GeometricShape::ShapeVerticesInfo Cylinder::computeBasicInfoCylinder (size_t meriCount) {

    GeometricShape::ShapeVertexAttributes infoAttributesCylinder;
    GeometricShape::IndicesBuffer indices;
    constexpr float r = 0.5f;
    // Create a Cylinder --------------------------------------------------------

    const float a2 = (360.0f / static_cast <float>(meriCount - 1)) *
                     static_cast <float>(M_PI) / 180.0f;

    infoAttributesCylinder.positions.emplace_back(0.f, 0.f, 0.f);
    infoAttributesCylinder.normals.emplace_back(0.f, -1.f, 0.f);
    infoAttributesCylinder.colors.emplace_back(1.f, 0.f, 0.f);
    infoAttributesCylinder.uvCoords.emplace_back(0.f, 0.f);

    infoAttributesCylinder.positions.emplace_back(0.f, 1.f, 0.f);
    infoAttributesCylinder.normals.emplace_back(0.f, 1.f, 0.f);
    infoAttributesCylinder.colors.emplace_back(0.f, 1.f, 0.f);
    infoAttributesCylinder.uvCoords.emplace_back(0.f, 0.f);


    const glm::vec3 initialPositionBase(r, 0.f, 0.f);
    const glm::vec3 initialPositionTop(r, 1.f, 0.f);
    infoAttributesCylinder.positions.push_back(initialPositionBase);
    infoAttributesCylinder.positions.push_back(initialPositionTop);
    infoAttributesCylinder.positions.push_back(initialPositionBase);
    infoAttributesCylinder.positions.push_back(initialPositionTop);

    infoAttributesCylinder.colors.emplace_back(1.f, 0.f, 0.f);
    infoAttributesCylinder.colors.emplace_back(0.f, 1.f, 0.f);
    infoAttributesCylinder.colors.emplace_back(1.f, 0.f, 0.f);
    infoAttributesCylinder.colors.emplace_back(0.f, 1.f, 0.f);

    infoAttributesCylinder.normals.push_back(glm::normalize(initialPositionBase));
    infoAttributesCylinder.normals.push_back(glm::normalize(initialPositionBase));
    infoAttributesCylinder.normals.emplace_back(0.f, -1.f, 0.f);
    infoAttributesCylinder.normals.emplace_back(0.f, 1.f, 0.f);

    infoAttributesCylinder.uvCoords.emplace_back(initialPositionBase.x, initialPositionBase.z);
    infoAttributesCylinder.uvCoords.emplace_back(initialPositionTop.x, initialPositionTop.z);
    infoAttributesCylinder.uvCoords.emplace_back(initialPositionBase.x, initialPositionBase.z);
    infoAttributesCylinder.uvCoords.emplace_back(initialPositionTop.x, initialPositionTop.z);

    for (unsigned int i = 1; i < meriCount; ++i) {
        const glm::mat4 rotation = glm::rotate(
            a2 * static_cast<float>(i),
            glm::vec3(0.f, 1.f, 0.f)
        );

        const glm::vec3 positionBase(rotation * glm::vec4(r, 0.f, 0.f, 1.f));
        const glm::vec3 positionTop(positionBase.x, 1.f, positionBase.z);

        for (unsigned int j = 0; j < 2; j++) {
            infoAttributesCylinder.positions.push_back(positionBase);
            infoAttributesCylinder.positions.push_back(positionTop);

            infoAttributesCylinder.colors.emplace_back(1.f, 0.f, 0.f);
            infoAttributesCylinder.colors.emplace_back(0.f, 1.f, 0.f);

            infoAttributesCylinder.uvCoords.emplace_back(positionBase.x, positionBase.z);
            infoAttributesCylinder.uvCoords.emplace_back(positionTop.x, positionTop.z);
        }

        infoAttributesCylinder.normals.push_back(glm::normalize(positionBase));
        infoAttributesCylinder.normals.push_back(glm::normalize(positionBase));
        infoAttributesCylinder.normals.emplace_back(0.f, -1.f, 0.f);
        infoAttributesCylinder.normals.emplace_back(0.f, 1.f, 0.f);
    }

    for (unsigned int i = 0; i < (meriCount - 1); ++i) {
        indices.push_back(0);
        indices.push_back(2 + (i + 1) * 4 + 2);
        indices.push_back(2 + i * 4 + 2);

        indices.push_back(1);
        indices.push_back(2 + i * 4 + 2 + 1);
        indices.push_back(2 + (i + 1) * 4 + 2 + 1);

        indices.push_back(2 + i * 4);
        indices.push_back(2 + (i + 1) * 4);
        indices.push_back(2 + i * 4 + 1);

        indices.push_back(2 + (i + 1) * 4);
        indices.push_back(2 + (i + 1) * 4 + 1);
        indices.push_back(2 + i * 4 + 1);
    }

    GeometricShape::ShapeVerticesInfo verticesInfo;
    verticesInfo.shapeVertexAttributes = infoAttributesCylinder;
    verticesInfo.indices = indices;
    return verticesInfo;
}


std::vector <glm::vec3> Cylinder::createCenterAndEdgeColorBuffer (
    const glm::vec3& customColorCenter,
    const glm::vec3& customColorEdge,
    size_t size) {

    std::vector <glm::vec3> customColorCube;
    customColorCube.push_back(customColorCenter);
    customColorCube.push_back(customColorCenter);
    for (size_t i = 2; i < size; ++i) {
        customColorCube.push_back(customColorEdge);
    }
    return customColorCube;
}

size_t Cylinder::levelOfDetail() const {
    return _meriCount;
}

std::vector <glm::vec3> Cylinder::genColors (const std::vector <glm::vec3>& colors) const {
    if (colors.size() == 2) {
        return createCenterAndEdgeColorBuffer(
            colors.at(0),    // customColorCenter
            colors.at(1),    // customColorEdge
            computeBasicInfoCylinder(_meriCount).shapeVertexAttributes.colors.size()
            );
    }
    if (colors.size() == 1) {
        return GeometricShape::createCustomColorBuffer(
            colors.at(0), // customColor
            computeBasicInfoCylinder(_meriCount).shapeVertexAttributes.colors.size()
            );
    }
    return computeBasicInfoCylinder(_meriCount).shapeVertexAttributes.colors;
}

std::vector <GLushort> Cylinder::genIndices() const {
    return computeBasicInfoCylinder(_meriCount).indices;
}

std::vector <glm::vec3> Cylinder::genNormals() const {
    return computeBasicInfoCylinder(_meriCount).shapeVertexAttributes.normals;
}

std::vector <glm::vec3> Cylinder::genPositions() const {
    return computeBasicInfoCylinder(_meriCount).shapeVertexAttributes.positions;
}

std::array<glm::vec3, 2> Cylinder::getCenterAndEdgeColor(const JBTypes::Color& color) {
    if (color == JBTypes::Color::Red) {
        return { glm::vec3 (1.f, 0.f, 0.f), glm::vec3 (0.5f, 0.f, 0.f) };
    }
    if (color == JBTypes::Color::Green) {
        return { glm::vec3 (0.f, 1.f, 0.f), glm::vec3 (0.f, 0.5f, 0.f) };
    }
    if (color == JBTypes::Color::Blue) {
        return { glm::vec3 (0.f, 0.f, 1.f), glm::vec3 (0.f, 0.f, 0.5f) };
    }
    if (color == JBTypes::Color::Orange) {
        return { glm::vec3 (1.f, 215.f / 255.f, 0.f), glm::vec3(150.f / 255.f, 75.f / 255.f, 0.f)};
    }
    if (color == JBTypes::Color::Yellow) {
        return { glm::vec3 (1.f, 1.f, 0.f), glm::vec3(1.f, 150.f / 255.f, 0.f)};
    }
    if (color == JBTypes::Color::White) {
        return { glm::vec3 (0.8f, 0.8f, 0.8f), glm::vec3(0.6f, 0.6f, 0.6f)};
    }

    constexpr float laserIntensity = 1.5f;
    // Lasers create the bloom effect, but we would like that
    // all laser colors have the same brightness;
    constexpr float redConeSensibilityCoeff = 0.3f;
    constexpr float greenConeSensibilityCoeff = 0.59f;
    constexpr float blueConeSensibilityCoeff = 0.11f;
    if (color == JBTypes::Color::ShinyRed) {
        const auto redColor = laserIntensity * glm::vec3(1.f / redConeSensibilityCoeff, 0.f, 0.f);
        return { redColor, redColor };
    }
    if (color == JBTypes::Color::ShinyBlue) {
        const auto blueColor = laserIntensity * glm::vec3(
            0.f,
            1.f / greenConeSensibilityCoeff,
            1.f / blueConeSensibilityCoeff
        );
        return { blueColor, blueColor };
    }
    if (color == JBTypes::Color::ShinyGreen) {
        const auto greenColor = laserIntensity * glm::vec3(0.f, 1.f / greenConeSensibilityCoeff, 0.f);
        return { greenColor, greenColor };
    }
    return {glm::vec3(0.f), glm::vec3(0.f)};
}
