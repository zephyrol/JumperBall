//
// Created by S.Morgenthaler on 03/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_MESHGEOMETRY_H
#define JUMPERBALLAPPLICATION_MESHGEOMETRY_H

#include "geometry/GeometricShape.h"
#include "scene/Displayable.h"

class MeshGeometry {

public:

    explicit MeshGeometry(
        VertexAttributes vertexAttributes,
        GeometricShape::IndicesBuffer indices
    );

    MeshGeometry(const MeshGeometry &meshGeometry) = delete;

    MeshGeometry &operator=(const MeshGeometry &meshGeometry) = delete;

    MeshGeometry(MeshGeometry &&meshGeometry) = default;

    MeshGeometry &operator=(MeshGeometry &&meshGeometry) = default;

    static MeshGeometry createInstance(
        const CstDisplayable_sptr &displayable,
        const vecCstGeometricShape_sptr &shapes,
        short dynamicsId
    );

    /**
     * Merge a mesh geometry into the current one moving it.
     * @param other The other that will be moved into the current one.
     */
    void merge(MeshGeometry &&other);

    /**
     * Extract the vertex attributes. Warning this is a side effect, the vertex attributes won't
     * exist anymore in the MeshGeometry
     */
    vecVertexAttributeBase_uptr extractVertexAttributes();

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


template<typename VertexAttribute, typename StaticValues>
std::vector<std::function<std::unique_ptr<VertexAttribute>()> >
MeshGeometry::createStaticVertexAttributeGenerationFunctions(
    size_t numberOfVertices,
    const StaticValues &staticValues
) {
    std::vector<std::function<std::unique_ptr<VertexAttribute>()> > staticVertexAttributeGenerationFunctions;
    for (const auto &staticValue: staticValues) {
        // Values are passed by copy, because the function will be called after the end of this current
        // function execution
        staticVertexAttributeGenerationFunctions.emplace_back([numberOfVertices, staticValue]() {
            std::vector<decltype(staticValue)> openGlValue(
                numberOfVertices,
                staticValue
            );
            return VertexAttributes::genVertexAttribute(std::move(openGlValue));
        });
    }
    return staticVertexAttributeGenerationFunctions;
}


#endif //JUMPERBALLAPPLICATION_MESHGEOMETRY_H
