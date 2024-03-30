/*
 * File: FrameBuffer.h
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "ShaderProgram.h"
#include "TextureSampler.h"

class FrameBuffer;

using FrameBuffer_uptr = std::unique_ptr<FrameBuffer>;
using CstFrameBuffer_uptr = std::unique_ptr<const FrameBuffer>;

class FrameBuffer {

public:
    explicit FrameBuffer(
        GLuint fboHandle,
        CstTextureSampler_uptr renderTexture
    );

    FrameBuffer(const FrameBuffer &frameBuffer) = delete;

    FrameBuffer &operator=(const FrameBuffer &) = delete;

    const CstTextureSampler_uptr& getRenderTexture() const;

    void bindFrameBuffer() const;

    virtual void clear() = 0;

    virtual ~FrameBuffer();

    static void bindDefaultFrameBuffer(GLint defaultFrameBuffer);

    static void setViewportSize(GLsizei resolutionX, GLsizei resolutionY);

    static void enableDepthTest();

    static void disableDepthTest();

    static void enableBlending();

    static void disableBlending();

protected:
    static GLuint createFrameBufferObject();

private:
    const GLuint _fboHandle;
    const CstTextureSampler_uptr _renderTexture;
};

#endif /* FRAMEBUFFER_H */
