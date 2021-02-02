/*
 * File: GeometricShape.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:07
 */

#ifndef GEOMETRICSHAPE_H
#define GEOMETRICSHAPE_H
#include "Shader.h"
#include "Utility.h"

class GeometricShape;
using GeometricShape_sptr = std::shared_ptr <GeometricShape>;
using CstGeometricShape_sptr = std::shared_ptr <const GeometricShape>;
using vecCstGeometricShape_sptr = std::vector <GeometricShape_sptr>;
using vecGeometricShape_sptr = std::vector <GeometricShape_sptr>;

class GeometricShape {
public:

// --CONSTRUCTORS & DESTRUCTORS--//
GeometricShape(
    const std::vector <glm::vec3>& positions,
    const std::vector <glm::vec3>& normals,
    const std::vector <glm::vec3>& colors,
    const std::vector <glm::vec2>& uvCoords,
    const glm::mat4& modelTransform = glm::mat4(1.f),
    const glm::mat4& normalsTransform = glm::mat4(1.f),
    const std::vector <GLushort>& indices = {});

// It's useless to have a copy of a shape with exactly the same transform
// and vertices, every attributes are const...
GeometricShape(const GeometricShape& geometricShape) = delete;
GeometricShape& operator= (const GeometricShape& geometricShape) = delete;

GeometricShape(GeometricShape&& geometricShape) = default;
GeometricShape& operator= (GeometricShape&& geometricShape) = default;


// Use this constructor to make a copy with a different transform
GeometricShape(const GeometricShape& geometricShape,
               const glm::mat4& modelTransform,
               const glm::mat4& normalsTransform);

virtual ~GeometricShape() = default;

const std::shared_ptr <const std::vector <glm::vec3> >& positions() const;
const std::shared_ptr <const std::vector <glm::vec3> >& normals() const;
const std::shared_ptr <const std::vector <glm::vec3> >& colors() const;
const std::shared_ptr <const std::vector <glm::vec2> >& uvCoords() const;
const std::shared_ptr <const std::vector <GLushort> >& indices() const;
virtual size_t levelOfDetail() const;

// --------STATIC METHODS-------//
static std::vector <glm::vec3> createCustomColorBuffer(
    const glm::vec3& customColor,
    size_t size);

private:

// --------ATTRIBUTES-----------//
const std::shared_ptr <const std::vector <glm::vec3> > _positions;
const std::shared_ptr <const std::vector <glm::vec3> > _normals;
const std::shared_ptr <const std::vector <glm::vec3> > _colors;
const std::shared_ptr <const std::vector <glm::vec2> > _uvCoords;
const std::shared_ptr <const std::vector <GLushort> > _indices;

std::shared_ptr <const std::vector <glm::vec3> > computePositions(
    const std::vector <glm::vec3>& positions, const glm::mat4& modelTransform) const;
std::shared_ptr <const std::vector <glm::vec3> > computeNormals(
    const std::vector <glm::vec3>& normals, const glm::mat4& normalsTransform) const;

};

#endif /* GEOMETRICSHAPE_H */
