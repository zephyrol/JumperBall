//
// Created by S.Morgenthaler on 21/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_DEPTHFRAMEBUFFER_H
#define JUMPERBALLAPPLICATION_DEPTHFRAMEBUFFER_H

#include "FrameBuffer.h"

class DepthFrameBuffer;

using DepthFrameBuffer_uptr = std::unique_ptr<DepthFrameBuffer>;

class DepthFrameBuffer : public FrameBuffer {
public:
    static DepthFrameBuffer_uptr createInstance(
        GLsizei resolutionX,
        GLsizei resolutionY
    );

    DepthFrameBuffer(
        CstTextureSampler_uptr renderTexture,
        CstGpuFrameBuffer_uptr gpuFrameBuffer
    );

    void clear() override;

    ~DepthFrameBuffer() override = default;
};


#endif //JUMPERBALLAPPLICATION_DEPTHFRAMEBUFFER_H
