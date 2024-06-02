//
// Created by S.Morgenthaler on 21/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_COLORABLEFRAMEBUFFER_H
#define JUMPERBALLAPPLICATION_COLORABLEFRAMEBUFFER_H

#include "FrameBuffer.h"
#include "RenderBuffer.h"

class ColorableFrameBuffer;
using ColorableFrameBuffer_uptr = std::unique_ptr<ColorableFrameBuffer>;

class ColorableFrameBuffer : public FrameBuffer {
   public:
    static ColorableFrameBuffer_uptr createInstance(GLsizei resolutionX,
                                                    GLsizei resolutionY,
                                                    bool useNearestFilter,
                                                    bool hasDepthBuffer,
                                                    std::unique_ptr<glm::vec3> clearColor = nullptr);

    ColorableFrameBuffer(CstTextureSampler_uptr renderTexture,
                         CstGpuFrameBuffer_uptr gpuFrameBuffer,
                         CstRenderBuffer_uptr depthBuffer,
                         std::unique_ptr<glm::vec3> clearColor);

    void clear() override;

   private:
    const std::unique_ptr<const glm::vec3> _clearColor;
};

#endif  // JUMPERBALLAPPLICATION_COLORABLEFRAMEBUFFER_H
