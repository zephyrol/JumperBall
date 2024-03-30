//
// Created by S.Morgenthaler on 30/03/2024.
//

#ifndef GPUFRAMEBUFFER_H
#define GPUFRAMEBUFFER_H
#include "Utility.h"

class GpuFrameBuffer;

using CstGpuFrameBuffer_uptr = std::unique_ptr<const GpuFrameBuffer>;
using vecCstGpuFrameBuffer_uptr = std::vector<CstGpuFrameBuffer_uptr>;

class GpuFrameBuffer {
public:
    GpuFrameBuffer();

    /**
     * Copy constructor and assignment are deleted because a OpenGL frame buffer id is unique.
     */
    GpuFrameBuffer(const GpuFrameBuffer &gpuFrameBuffer) = delete;
    GpuFrameBuffer &operator=(const GpuFrameBuffer &gpuFrameBuffer) = delete;

    /**
     * Move constructor and assignment are deleted because the destructor free the GPU memory.
     */
    GpuFrameBuffer(GpuFrameBuffer &&gpuFrameBuffer) = delete;
    GpuFrameBuffer &operator=(GpuFrameBuffer &&gpuFrameBuffer) = delete;

    /**
     * Bind a frame buffer from its id.
     */
    void bind() const;

    ~GpuFrameBuffer();

    static void bindDefaultFrameBuffer(GLint defaultFrameBuffer);

private:
    const GLuint _frameBufferId;
};


#endif //GPUFRAMEBUFFER_H
