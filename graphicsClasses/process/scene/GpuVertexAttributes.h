//
// Created by Sebastien Morgenthaler on 09/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_GPUVERTEXATTRIBUTES_H
#define JUMPERBALLAPPLICATION_GPUVERTEXATTRIBUTES_H

#include "process/mesh/vertexAttribute/VertexAttributeBase.h"

class GpuVertexAttributes;

using GpuVertexAttributes_sptr = std::shared_ptr<GpuVertexAttributes>;
using CstGpuVertexAttributes_sptr = std::shared_ptr<const GpuVertexAttributes>;
using vecCstGpuVertexAttributes_sptr = std::vector<CstGpuVertexAttributes_sptr>;
using vecGpuVertexAttributes_sptr = std::vector<GpuVertexAttributes_sptr>;

class GpuVertexAttributes {

public:
    GpuVertexAttributes(
        GLuint vertexBufferObject,
        const CstVertexAttributeBase_uptr& vertexAttribute,
        GLuint index
    );

    void freeGPUMemory();

    void bind() const;

    void unbind() const;

private:
    const GLuint _vertexBufferObject;
    const GLuint _index;
    const std::function<void(GLuint)> _vertexAttribPointerFunc;
};


#endif //JUMPERBALLAPPLICATION_GPUVERTEXATTRIBUTES_H