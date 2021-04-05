/*
 * File: GeometricShape.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:07
 */

#include "GeometricShape.h"

GeometricShape::GeometricShape(const glm::mat4& modelTransform,
                               const glm::mat4& normalsTransform,
                               std::vector <glm::vec3>&& customColors
                               ):
    _modelTransform(modelTransform),
    _normalsTransform(normalsTransform),
    _customColors(std::move(customColors)) {

}

GeometricShape::ShapeVertexAttributes GeometricShape::genVertexAttributes() const {
    const auto computePositions =
        [this] ()->std::vector <glm::vec3> {
            std::vector <glm::vec3> computedPositions;

            const std::vector <glm::vec3> positions = genPositions();
            for (const glm::vec3& position : positions) {
                computedPositions.push_back(_modelTransform * glm::vec4(position, 1.f));
            }
            return computedPositions;
        };

    const auto computeNormals =
        [this] ()->std::vector <glm::vec3> {
            std::vector <glm::vec3> computedNormals;

            const std::vector <glm::vec3> normals = genNormals();
            for (const glm::vec3& normal : normals) {
                computedNormals.push_back(_normalsTransform * glm::vec4(normal, 1.f));
            }
            return computedNormals;
        };

    GeometricShape::ShapeVertexAttributes shapeVertexAttributes;
    shapeVertexAttributes.positions = computePositions();
    shapeVertexAttributes.colors = genColors(_customColors);
    shapeVertexAttributes.normals = computeNormals();
    shapeVertexAttributes.uvCoords = genUvCoords();
    return shapeVertexAttributes;
}

size_t GeometricShape::numberOfVertices() const {
    return genPositions().size();
}

size_t GeometricShape::levelOfDetail() const {
    return 0;
}

GeometricShape::ShapeVerticesInfo GeometricShape::genShapeVerticesInfo() const {
    GeometricShape::ShapeVerticesInfo shapeVerticesInfo;
    shapeVerticesInfo.shapeVertexAttributes = genVertexAttributes();
    shapeVerticesInfo.indices = genIndices();
    return shapeVerticesInfo;
}

void GeometricShape::concatShapeVerticesInfo (
    GeometricShape::ShapeVerticesInfo& current,
    const GeometricShape::ShapeVerticesInfo& other) {
    GeometricShape::ShapeVertexAttributes& currentShapeVertexAttributes = current.shapeVertexAttributes;
    concatIndices(current.indices, other.indices, currentShapeVertexAttributes.positions.size());
    concatAttributes(currentShapeVertexAttributes, other.shapeVertexAttributes);
}

std::vector <glm::vec3> GeometricShape::createCustomColorBuffer (
    const glm::vec3& customColor,
    size_t size) {
    std::vector <glm::vec3> customColorCube(size);
    for (glm::vec3& color : customColorCube) {
        color = customColor;
    }
    return customColorCube;
}

std::vector <GLushort> GeometricShape::genIndices() const {
    const size_t nbOfVertices = numberOfVertices();
    std::vector <GLushort> indices(nbOfVertices);
    for (size_t i = 0; i < nbOfVertices; ++i) {
        indices.at(i) = i;
    }
    return indices;
}

void GeometricShape::concatIndices (
    IndicesBuffer& currentIndices,
    const IndicesBuffer& otherIndices,
    size_t offset
    ) {
    IndicesBuffer newIndices = otherIndices;
    for (GLushort& newIndice : newIndices) {
        newIndice += static_cast <GLushort>(offset);
    }
    Utility::concatVector(currentIndices, newIndices);
}

std::vector <glm::vec3> GeometricShape::genNormals() const {
    return {};
}

std::vector <glm::vec3> GeometricShape::genColors (const std::vector <glm::vec3>&) const {
    return {};
}

std::vector <glm::vec2> GeometricShape::genUvCoords() const {
    return {};
}

void GeometricShape::concatAttributes (
    GeometricShape::ShapeVertexAttributes& current,
    const GeometricShape::ShapeVertexAttributes& other) {
    Utility::concatVector(current.positions, other.positions);
    Utility::concatVector(current.normals, other.normals);
    Utility::concatVector(current.colors, other.colors);
    Utility::concatVector(current.uvCoords, other.uvCoords);
}
