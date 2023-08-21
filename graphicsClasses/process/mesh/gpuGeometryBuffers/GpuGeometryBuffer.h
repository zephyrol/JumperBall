//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_GPUGEOMETRYBUFFER_H
#define JUMPERBALLAPPLICATION_GPUGEOMETRYBUFFER_H

#include "Utility.h"

class GpuGeometryBuffer;

using GpuGeometryBuffer_sptr = std::shared_ptr<GpuGeometryBuffer>;
using CstGpuGeometryBuffer_sptr = std::shared_ptr<const GpuGeometryBuffer>;
using vecCstGpuGeometryBuffer_sptr = std::vector<CstGpuGeometryBuffer_sptr>;
using vecGpuGeometryBuffer_sptr = std::vector<GpuGeometryBuffer_sptr>;

class GpuGeometryBuffer {
public:

    explicit GpuGeometryBuffer(
        GLuint bufferObject
    );

    void freeGPUMemory();

    virtual ~GpuGeometryBuffer() = default;

protected:
    const GLuint _bufferObject;

    static GLuint createBufferObject();
};


#endif // JUMPERBALLAPPLICATION_GPUGEOMETRYBUFFER_H
