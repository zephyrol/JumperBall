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

vecVertexAttributeBase_uptr GeometricShape::genVertexAttributes() const {
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

    const std::vector<std::function<VertexAttributeBase_uptr()> > vertexAttributeGenerationFunctions{
        [&computePositions]() { return genVertexAttribute(computePositions()); },
        [this]() { return genVertexAttribute(genColors(_customColors)); }
        //[&computeNormals]() { return genVertexAttribute(computeNormals()); },
        //[this]() { return genVertexAttribute(genUvCoords(_customUvs)); }
    };

    return VertexAttributeBase::genAndFilterVertexAttributes(vertexAttributeGenerationFunctions);
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
        indices[i] = i;
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

template<typename T>
VertexAttribute_uptr<T> GeometricShape::genVertexAttribute(std::vector<T> &&vertexAttributeData) {
    if (vertexAttributeData.empty()) {
        return nullptr;
    }
    return std::unique_ptr<VertexAttribute<T>>(
        new VertexAttribute<T>(std::move(vertexAttributeData), GL_FLOAT)
    );
}
