//
// Created by S.Morgenthaler on 21/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_COLORABLEFRAMEBUFFER_H
#define JUMPERBALLAPPLICATION_COLORABLEFRAMEBUFFER_H


#include "FrameBuffer.h"

class ColorableFrameBuffer;
using ColorableFrameBuffer_uptr = std::unique_ptr<ColorableFrameBuffer>;

class ColorableFrameBuffer : public FrameBuffer {
public:
    static ColorableFrameBuffer_uptr createInstance(
        GLsizei resolutionX,
        GLsizei resolutionY,
        bool isHDR,
        bool hasDepthBuffer,
        std::unique_ptr<glm::vec3> clearColor = nullptr
    );

    ColorableFrameBuffer(
        GLuint fboHandle,
        GLuint renderTexture,
        std::unique_ptr<const GLuint> depthBuffer,
        std::unique_ptr<glm::vec3> clearColor
    );

    void freeGPUMemory() override;

    void clear() override;

private:
    const std::unique_ptr<const GLuint> _depthBuffer;
    const std::unique_ptr<const glm::vec3> _clearColor;

};


#endif //JUMPERBALLAPPLICATION_COLORABLEFRAMEBUFFER_H
