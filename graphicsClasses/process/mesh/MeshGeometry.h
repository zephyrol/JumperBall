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
        vecVertexAttributeBase_uptr vertexAttributes,
        GeometricShape::IndicesBuffer indices
    );

    MeshGeometry(MeshGeometry&& meshGeometry) = default;
    MeshGeometry& operator=(MeshGeometry&& meshGeometry) = default;

    static MeshGeometry createInstance(
        const CstDisplayable_sptr &displayable,
        const vecCstGeometricShape_sptr &shapes
    );

    /**
     * Merge a mesh geometry into the current one moving it.
     * @param other The other that will be moved into the current one.
     */
    void merge(MeshGeometry &&other);

    const vecVertexAttributeBase_uptr &vertexAttributes();

    const GeometricShape::IndicesBuffer &indices() const;

private:

    GeometricShape::IndicesBuffer &&moveIndices();

    vecVertexAttributeBase_uptr &&moveVertexAttributes();

    vecVertexAttributeBase_uptr _vertexAttributes;
    GeometricShape::IndicesBuffer _indices;

    template<typename OpenGLType, typename RawType>
    static VertexAttribute_uptr<OpenGLType> genStaticVertexAttribute(
        const Displayable::StaticValues<RawType> &staticVertexAttributeData,
        GLenum glTypeEnum
    );

};


#endif //JUMPERBALLAPPLICATION_MESHGEOMETRY_H
