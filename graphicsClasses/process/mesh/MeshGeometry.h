//
// Created by S.Morgenthaler on 03/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_MESHGEOMETRY_H
#define JUMPERBALLAPPLICATION_MESHGEOMETRY_H

#include "geometry/GeometricShape.h"
#include "scene/Displayable.h"
#include "VertexAttribute.h"

class MeshGeometry {

public:

    explicit MeshGeometry(const CstDisplayable_sptr &displayable, const vecCstGeometricShape_sptr &shapes);

    /**
     * Merge a mesh geometry into the current one moving it.
     * @param other The other that will be moved into the current one.
     */
    void merge(MeshGeometry &&other);

private:


    static vecVertexAttributeBase_uptr genVertexAttributes(
        const CstDisplayable_sptr &displayable,
        const vecCstGeometricShape_sptr &shapes
    );

    static GeometricShape::IndicesBuffer genIndices(
        const vecCstGeometricShape_sptr &shapes,
        const vecVertexAttributeBase_uptr& vertexAttributes
    );

    GeometricShape::IndicesBuffer && moveIndices();

    vecVertexAttributeBase_uptr && moveVertexAttributes();

    vecVertexAttributeBase_uptr _vertexAttributes;
    GeometricShape::IndicesBuffer _indices;

    template<typename RawType, typename OpenGLType>
    static void convertAttributesToOpenGLFormat(
        const Displayable::StaticValues<RawType> &rawValues,
        std::vector<OpenGLType> &openGLValues
    );

    template<typename OpenGLType, typename RawType>
    static VertexAttribute_uptr<OpenGLType> genStaticVertexAttribute(
        const Displayable::StaticValues<RawType> &staticVertexAttributeData,
        GLenum glTypeEnum
    );

};


#endif //JUMPERBALLAPPLICATION_MESHGEOMETRY_H
