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
        GLuint fboHandle,
        GLuint renderTexture,
        GLuint depthBuffer
    );

    void freeGPUMemory() override;

    void clear() override;

private:
    const GLuint _depthBuffer;
};


#endif //JUMPERBALLAPPLICATION_DEPTHFRAMEBUFFER_H
