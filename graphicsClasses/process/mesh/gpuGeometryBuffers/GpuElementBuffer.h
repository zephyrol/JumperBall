//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_GPUELEMENTBUFFER_H
#define JUMPERBALLAPPLICATION_GPUELEMENTBUFFER_H
#include "GpuGeometryBuffer.h"
#include "geometry/GeometricShape.h"

class GpuElementBuffer;

using GpuElementBuffer_sptr = std::shared_ptr<GpuElementBuffer>;
using CstGpuElementBuffer_sptr = std::shared_ptr<const GpuElementBuffer>;
using vecCstGpuElementBuffer_sptr = std::vector<CstGpuElementBuffer_sptr>;
using vecGpuElementBuffer_sptr = std::vector<GpuElementBuffer_sptr>;

class GpuElementBuffer: public GpuGeometryBuffer {

public:
    explicit GpuElementBuffer(
        GLuint elementBufferObject
    );

    /**
     * Create Gpu Vertex Buffer in the current VAO.
     * Warning: The VAO needs to be bound before and the new buffer object will be stayed bound.
     * @param vertexAttribute Core vertex attribute.
     * @param index VBO index in the VAO.
     * @return New Gpu Vertex Buffer.
     */
    static GpuElementBuffer_sptr createInstance(
        const GeometricShape::IndicesBuffer& indices
    );

};


#endif //JUMPERBALLAPPLICATION_GPUELEMENTBUFFER_H
