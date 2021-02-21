/*
 * File: GeometricShape.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:07
 */

#include "GeometricShape.h"

GeometricShape::GeometricShape(const std::vector <glm::vec3>& positions,
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

}

const std::shared_ptr <const std::vector <glm::vec3> >& GeometricShape::positions() const {
    return _positions;
}

const std::shared_ptr <const std::vector <glm::vec3> >& GeometricShape::normals() const {
    return _normals;
}

const std::shared_ptr <const std::vector <glm::vec3> >& GeometricShape::colors() const {
    return _colors;
}

const std::shared_ptr <const std::vector <glm::vec2> >& GeometricShape::uvCoords() const {
    return _uvCoords;
}

const std::shared_ptr <const std::vector <GLushort> >& GeometricShape::indices() const {
    return _indices;
}

size_t GeometricShape::numberOfVertices() const {
    return _numberOfVertices;
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

std::shared_ptr <const std::vector <glm::vec3> > GeometricShape::computePositions (
    const std::vector <glm::vec3>& positions, const glm::mat4& modelTransform) const {

    std::vector <glm::vec3> computedPositions;
    for (const glm::vec3& position : positions) {
        computedPositions.push_back(modelTransform * glm::vec4(position, 1.f));
    }

    return std::make_shared <std::vector <glm::vec3> >(std::move(computedPositions));
}

std::shared_ptr <const std::vector <glm::vec3> > GeometricShape::computeNormals (
    const std::vector <glm::vec3>& normals, const glm::mat4& normalsTransform) const {

    std::vector <glm::vec3> computedNormals;
    for (const glm::vec3& normal : normals) {
        computedNormals.push_back(normalsTransform * glm::vec4(normal, 1.f));
    }

    return std::make_shared <std::vector <glm::vec3> >(std::move(computedNormals));
}

std::shared_ptr <const std::vector <GLushort> > GeometricShape::genIndices() const {
    std::vector <GLushort> indices(_numberOfVertices);
    for (size_t i = 0; i < _numberOfVertices; ++i) {
        indices.at(i) = i;
    }
    return std::make_shared <std::vector <GLushort> >(std::move(indices));
}


/*void GeometricShape::setVerticesData() const {

   glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(0));
   glBufferData(GL_ARRAY_BUFFER, _positions->size() * 3 * sizeof(GLfloat),
   _positions->data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(1));
   glBufferData(GL_ARRAY_BUFFER, _colors->size() * 3 * sizeof(GLfloat),
   _colors->data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(2));
   glBufferData(GL_ARRAY_BUFFER, _normals->size() * 3 * sizeof(GLfloat),
   _normals->data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjects->at(3));
   glBufferData(GL_ARRAY_BUFFER, _uvCoords->size() * 2 * sizeof(GLfloat),
   _uvCoords->data(), GL_STATIC_DRAW);

   }

   void GeometricShape::bind() const {
   glBindVertexArray(*_vertexArrayObject);
   }

   void GeometricShape::draw() const {

   if (_elementBufferObject && _indices) {
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_elementBufferObject);
   glDrawElements(GL_TRIANGLES, static_cast <GLsizei>(_indices->size()),
   GL_UNSIGNED_SHORT, nullptr);
   } else {
   constexpr int offset = 0;
   glDrawArrays(GL_TRIANGLES, offset,
   static_cast <GLsizei>(_positions->size()));
   }


   }*/
