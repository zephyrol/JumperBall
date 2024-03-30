/*
 * File: FrameBuffer.h
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "GpuFrameBuffer.h"
#include "RenderBuffer.h"
#include "TextureSampler.h"

class FrameBuffer;

using FrameBuffer_uptr = std::unique_ptr<FrameBuffer>;
using CstFrameBuffer_uptr = std::unique_ptr<const FrameBuffer>;

class FrameBuffer {
public:
    explicit FrameBuffer(
        CstTextureSampler_uptr renderTexture,
        CstRenderBuffer_uptr renderBuffer,
        CstGpuFrameBuffer_uptr gpuFrameBuffer
    );

    FrameBuffer(const FrameBuffer &frameBuffer) = delete;

    FrameBuffer &operator=(const FrameBuffer &) = delete;

    const CstTextureSampler_uptr &getRenderTexture() const;

    void bindFrameBuffer() const;

    virtual void clear() = 0;

    virtual ~FrameBuffer() = default;

    static void setViewportSize(GLsizei resolutionX, GLsizei resolutionY);

    static void enableDepthTest();

    static void disableDepthTest();

    static void enableBlending();

    static void disableBlending();

protected:
    const CstTextureSampler_uptr _renderTexture;
    const CstRenderBuffer_uptr _renderBuffer;
    const CstGpuFrameBuffer_uptr _gpuFrameBuffer;
};

#endif /* FRAMEBUFFER_H */
