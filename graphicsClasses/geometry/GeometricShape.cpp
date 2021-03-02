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

/*GeometricShape::GeometricShape(const std::vector <glm::vec3>& positions,
                               const std::vector <glm::vec3>& normals,
                               const std::vector <glm::vec3>& colors,
                               const std::vector <glm::vec2>& uvCoords,
                               const glm::mat4& modelTransform,
                               const glm::mat4& normalsTransform,
                               const std::vector <GLushort>& indices
                               ):
    _positions(positions.empty()
               ? nullptr
               : computePositions(positions, modelTransform)),
    // : std::make_shared <std::vector <glm::vec3> >(positions)),
    _normals(normals.empty()
             ? nullptr
             : computeNormals(normals, normalsTransform)),
    // : std::make_shared <std::vector <glm::vec3> >(normals)),
    _colors(colors.empty()
            ? nullptr
            : std::make_shared <std::vector <glm::vec3> >(colors)),
    _uvCoords(uvCoords.empty()
              ? nullptr
              : std::make_shared <std::vector <glm::vec2> >(uvCoords)),
    _numberOfVertices(_positions ? _positions->size() : 0),
    _indices(indices.empty()
             ? genIndices()
             : std::make_shared <std::vector <GLushort> >(indices)) {
   }

   GeometricShape::GeometricShape(const GeometricShape& geometricShape,
                               const glm::mat4& modelTransform,
                               const glm::mat4& normalsTransform):
    _positions(computePositions(*geometricShape.positions(), modelTransform)),
    _normals(computeNormals(*geometricShape.normals(), normalsTransform)),
    _colors(geometricShape.colors()),
    _uvCoords(geometricShape.uvCoords()),
    _numberOfVertices(_positions ? _positions->size() : 0),
    _indices(geometricShape.indices()) {

   }*/

GeometricShape::ShapeVertexAttributes GeometricShape::genVertexAttributes() const {
    const auto computePositions =
        [this] ()->std::vector <glm::vec3> {
            std::vector <glm::vec3> computedPositions;

            const std::vector <glm::vec3> positions = genPositions();
            for (const glm::vec3& position : positions) {
                computedPositions.push_back(_modelTransform *
                                            glm::vec4(position, 1.f));
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
    shapeVertexAttributes.indices = genIndices();
    return shapeVertexAttributes;
}

size_t GeometricShape::numberOfVertices() const {
    return genPositions().size();
}

size_t GeometricShape::levelOfDetail() const {
    return 0;
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
    std::vector <GLushort>& currentIndices,
    const std::vector <GLushort>& otherIndices,
    size_t offset
    ) {
    std::vector <GLushort> newIndices = otherIndices;
    for (GLushort& newIndice : newIndices) {
        newIndice += static_cast <GLushort>(offset);
    }
    Utility::concatVector(currentIndices, newIndices);
}

GeometricShape::ShapeVertexAttributes GeometricShape::concatAttributes (
    const GeometricShape::ShapeVertexAttributes& current,
    const GeometricShape::ShapeVertexAttributes& other) {
    GeometricShape::ShapeVertexAttributes shapeVertexAttributes = current;
    concatIndices(shapeVertexAttributes.indices, other.indices, shapeVertexAttributes.positions.size());
    Utility::concatVector(shapeVertexAttributes.positions, other.positions);
    Utility::concatVector(shapeVertexAttributes.normals, other.normals);
    Utility::concatVector(shapeVertexAttributes.colors, other.colors);
    Utility::concatVector(shapeVertexAttributes.uvCoords, other.uvCoords);
    return shapeVertexAttributes;
}
