/*
 * File: GeometricShape.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:07
 */

#include "GeometricShape.h"

GeometricShape::GeometricShape(
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform,
    std::vector<glm::vec3> &&customColors,
    std::vector<glm::vec2> &&customUvs
) :
    _modelTransform(modelTransform),
    _normalsTransform(normalsTransform),
    _customColors(std::move(customColors)),
    _customUvs(std::move(customUvs)) {

}

VertexAttributes GeometricShape::genVertexAttributes() const {
    const auto computePositions = [this]() {
        std::vector<glm::vec3> computedPositions;

        const auto positions = genPositions();
        for (const auto &position: positions) {
            computedPositions.emplace_back(_modelTransform * glm::vec4(position, 1.f));
        }
        return computedPositions;
    };

    const auto computeNormals = [this]() {
        std::vector<glm::vec3> computedNormals;

        const auto normals = genNormals();
        for (const auto &normal: normals) {
            computedNormals.emplace_back(_normalsTransform * glm::vec4(normal, 1.f));
        }
        return computedNormals;
    };

    const std::vector<std::function<VertexAttributeVec3_uptr()> > attributesVec3GenerationFunctions{
        [&computePositions]() { return VertexAttributes::genVertexAttribute(computePositions()); },
        [this]() { return VertexAttributes::genVertexAttribute(genColors(_customColors)); },
        [&computeNormals]() { return VertexAttributes::genVertexAttribute(computeNormals()); }
    };

    const std::vector<std::function<VertexAttributeVec2_uptr()> > attributesVec2GenerationFunctions{
        [this]() { return VertexAttributes::genVertexAttribute(genUvCoords(_customUvs)); }
    };

    return VertexAttributes(
        VertexAttributeBase::genAndFilter(attributesVec3GenerationFunctions),
        VertexAttributeBase::genAndFilter(attributesVec2GenerationFunctions),
        {},
        {},
        {}
    );
}

std::vector<glm::vec3> GeometricShape::createCustomColorBuffer(
    const glm::vec3 &customColor,
    size_t size
) {
    std::vector<glm::vec3> customColorCube(size);
    for (glm::vec3 &color: customColorCube) {
        color = customColor;
    }
    return customColorCube;
}

std::vector<GLushort> GeometricShape::genIndices() const {
    const size_t nbOfVertices = genPositions().size();
    std::vector<GLushort> indices(nbOfVertices);
    for (size_t i = 0; i < nbOfVertices; ++i) {
        indices[i] = static_cast<GLushort>(i);
    }
    return indices;
}

std::vector<glm::vec3> GeometricShape::genNormals() const {
    return {};
}

std::vector<glm::vec3> GeometricShape::genColors(const std::vector<glm::vec3> &) const {
    return {};
}

std::vector<glm::vec2> GeometricShape::genUvCoords(const std::vector<glm::vec2> &) const {
    return {};
}
