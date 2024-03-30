//
// Created by S.Morgenthaler on 30/03/2024.
//

#ifndef GPUBUFFER_H
#define GPUBUFFER_H
#include "Utility.h"


class GpuBuffer;

using CstGpuBuffer_uptr = std::unique_ptr<const GpuBuffer>;
using vecCstGpuBuffer_uptr = std::vector<CstGpuBuffer_uptr>;

class GpuBuffer {
public:
    GpuBuffer();

    /**
     * Copy constructor and assignment are deleted because a OpenGL buffer id is unique.
     */
    GpuBuffer(const GpuBuffer &gpuBuffer) = delete;
    GpuBuffer &operator=(const GpuBuffer &gpuBuffer) = delete;

    /**
     * Move constructor and assignment are deleted because the destructor free the GPU memory.
     */
    GpuBuffer(GpuBuffer &&gpuBuffer) = delete;
    GpuBuffer &operator=(GpuBuffer &&gpuBuffer) = delete;

    /**
    * Get OpenGL render buffer Id. Use this getter ONLY to reference the ID to OpenGL API.
    * Do not store it anywhere!
    */
    GLuint getId() const;

    ~GpuBuffer();

private:
    const GLuint _bufferId;
};


#endif //GPUBUFFER_H
