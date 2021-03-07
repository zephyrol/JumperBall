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
using vecCstGeometricShape_sptr = std::vector <CstGeometricShape_sptr>;
using vecGeometricShape_sptr = std::vector <GeometricShape_sptr>;

class GeometricShape {
public:

struct ShapeVertexAttributes {
    std::vector <glm::vec3> positions = {};
    std::vector <glm::vec3> normals = {};
    std::vector <glm::vec3> colors = {};
    std::vector <glm::vec2> uvCoords {};
};

using IndicesBuffer = std::vector <GLushort>;

struct ShapeVerticesInfo {
    ShapeVertexAttributes shapeVertexAttributes;
    IndicesBuffer indices = {};
};

GeometricShape(
    const glm::mat4& modelTransform,
    const glm::mat4& normalsTransform,
    std::vector <glm::vec3>&& customColors
    );

// It's useless to have a copy of a shape with exactly the same transform
// and vertices, every attributes are const...
GeometricShape(const GeometricShape& geometricShape) = delete;
GeometricShape& operator= (const GeometricShape& geometricShape) = delete;

GeometricShape(GeometricShape&& geometricShape) = default;
GeometricShape& operator= (GeometricShape&& geometricShape) = default;
virtual ~GeometricShape() = default;

virtual size_t numberOfVertices() const;

virtual size_t levelOfDetail() const;

ShapeVerticesInfo genShapeVerticesInfo() const;
static void concatShapeVerticesInfo(
    ShapeVerticesInfo& current,
    const ShapeVerticesInfo& other
    );

protected:

static std::vector <glm::vec3> createCustomColorBuffer(
    const glm::vec3& customColor,
    size_t size);

private:

ShapeVertexAttributes genVertexAttributes() const;
virtual std::vector <GLushort> genIndices() const;

static void concatAttributes(ShapeVertexAttributes& current, const ShapeVertexAttributes& other);
static void concatIndices(
    IndicesBuffer& currentIndices,
    const IndicesBuffer& otherIndices,
    size_t offset
    );


const glm::mat4 _modelTransform;
const glm::mat4 _normalsTransform;
const std::vector <glm::vec3> _customColors;
virtual std::vector <glm::vec3> genPositions() const = 0;
virtual std::vector <glm::vec3> genNormals() const = 0;
virtual std::vector <glm::vec3> genColors(const std::vector <glm::vec3>& colors) const = 0;
virtual std::vector <glm::vec2> genUvCoords() const = 0;

};

#endif /* GEOMETRICSHAPE_H */
