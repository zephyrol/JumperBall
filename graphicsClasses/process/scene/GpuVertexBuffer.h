//
// Created by S.Morgenthaler on 09/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_GPUVERTEXBUFFER_H
#define JUMPERBALLAPPLICATION_GPUVERTEXBUFFER_H

#include "process/mesh/vertexAttribute/VertexAttributeBase.h"

class GpuVertexBuffer;

using GpuVertexBuffer_sptr = std::shared_ptr<GpuVertexBuffer>;
using CstGpuVertexBuffer_sptr = std::shared_ptr<const GpuVertexBuffer>;
using vecCstGpuVertexBuffer_sptr = std::vector<CstGpuVertexBuffer_sptr>;
using vecGpuVertexBuffer_sptr = std::vector<GpuVertexBuffer_sptr>;

class GpuVertexBuffer {

public:

    explicit GpuVertexBuffer(
        GLuint vertexBufferObject
    );

    /**
     * Create Gpu Vertex Buffer in the current VAO.
     * Warning: The VAO needs to be bound before and the new buffer object will be stayed bound.
     * @param vertexAttribute Core vertex attribute.
     * @param index VBO index in the VAO.
     * @return New Gpu Vertex Buffer.
     */
    static GpuVertexBuffer_sptr createInstance(
        const CstVertexAttributeBase_uptr& vertexAttribute,
        GLuint index
    );

    void freeGPUMemory();

private:
    const GLuint _vertexBufferObject;
};


#endif //JUMPERBALLAPPLICATION_GPUVERTEXBUFFER_H
