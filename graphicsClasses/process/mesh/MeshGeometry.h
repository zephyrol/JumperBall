//
// Created by S.Morgenthaler on 03/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_MESHGEOMETRY_H
#define JUMPERBALLAPPLICATION_MESHGEOMETRY_H

#include "geometry/GeometricShape.h"
#include "scene/Displayable.h"
#include "process/mesh/vertexAttribute/VertexAttribute.h"

class MeshGeometry {

public:

    explicit MeshGeometry(
        VertexAttributes vertexAttributes,
        GeometricShape::IndicesBuffer indices
    );

    MeshGeometry(MeshGeometry &&meshGeometry) = default;

    MeshGeometry &operator=(MeshGeometry &&meshGeometry) = default;

    static MeshGeometry createInstance(
        const CstDisplayable_sptr &displayable,
        const vecCstGeometricShape_sptr &shapes
    );

    /**
     * Merge a mesh geometry into the current one moving it.
     * @param other The other that will be moved into the current one.
     */
    void merge(MeshGeometry &&other);

    const VertexAttributes &vertexAttributes();

    const GeometricShape::IndicesBuffer &indices() const;

private:

    GeometricShape::IndicesBuffer &&moveIndices();

    VertexAttributes &&moveVertexAttributes();

    VertexAttributes _vertexAttributes;
    GeometricShape::IndicesBuffer _indices;

    /**
     * For a specific type of static vertex (for example vec2, or int), get several functions used to
     * generate vertex attribute pointers. A function is created for each static value of the same type.
     * @tparam VertexAttribute Type of the vertex attributes created by the generation functions.
     * @tparam StaticValues Type of the static values.
     * @param numberOfVertices For N vertices, each static value is duplicated N times in the vertex
     *  attributes.
     * @param staticValues Vector containing static values.
     * @return The list of functions used to generate the vertex attributes.
     */
    template<typename VertexAttribute, typename StaticValues>
    static std::vector<std::function<std::unique_ptr<VertexAttribute>()> >
    createStaticVertexAttributeGenerationFunctions(
        size_t numberOfVertices,
        const StaticValues &staticValues
    );
};


#endif //JUMPERBALLAPPLICATION_MESHGEOMETRY_H
