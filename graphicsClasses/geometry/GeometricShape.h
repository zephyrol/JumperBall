/*
 * File: GeometricShape.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:07
 */

#ifndef GEOMETRICSHAPE_H
#define GEOMETRICSHAPE_H

#include "Shader.h"
#include "process/mesh/vertexAttribute/VertexAttribute.h"

class GeometricShape;

using GeometricShape_sptr = std::shared_ptr<GeometricShape>;
using CstGeometricShape_sptr = std::shared_ptr<const GeometricShape>;
using vecCstGeometricShape_sptr = std::vector<CstGeometricShape_sptr>;
using vecGeometricShape_sptr = std::vector<GeometricShape_sptr>;

class GeometricShape {
public:

    using IndicesBuffer = std::vector<GLushort>;
    GeometricShape(
        const glm::mat4 &modelTransform,
        const glm::mat4 &normalsTransform,
        std::vector<glm::vec3> &&customColors,
        std::vector<glm::vec2> &&customUvs
    );

    // It's useless to have a copy of a shape with exactly the same transform
    // and vertices, every attributes are const...
    GeometricShape(const GeometricShape &geometricShape) = delete;

    GeometricShape &operator=(const GeometricShape &geometricShape) = delete;

    virtual ~GeometricShape() = default;

    vecVertexAttributeBase_uptr genVertexAttributes() const;

    using IndexType = GLushort;
    virtual std::vector<IndexType> genIndices() const;
protected:

    static std::vector<glm::vec3> createCustomColorBuffer(
        const glm::vec3 &customColor,
        size_t size);

private:

    const glm::mat4 _modelTransform;
    const glm::mat4 _normalsTransform;
    const std::vector<glm::vec3> _customColors;
    const std::vector<glm::vec2> _customUvs;

    template<typename T>
    static VertexAttribute_uptr<T> genVertexAttribute(std::vector<T>&& vertexAttributeData);

    // Positions are required
    virtual std::vector<glm::vec3> genPositions() const = 0;

    virtual std::vector<glm::vec3> genNormals() const;

    virtual std::vector<glm::vec3> genColors(const std::vector<glm::vec3> &colors) const;

    virtual std::vector<glm::vec2> genUvCoords(const std::vector<glm::vec2> &uvs) const;

};

#endif /* GEOMETRICSHAPE_H */
