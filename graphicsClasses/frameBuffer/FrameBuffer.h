/*
 * File: FrameBuffer.h
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "ShaderProgram.h"

class FrameBuffer;

using FrameBuffer_uptr = std::unique_ptr<FrameBuffer>;
using CstFrameBuffer_uptr = std::unique_ptr<const FrameBuffer>;

class FrameBuffer {

public:
    explicit FrameBuffer(
        GLuint fboHandle,
        GLuint renderTexture
    );

    FrameBuffer(const FrameBuffer &frameBuffer) = delete;

    FrameBuffer &operator=(const FrameBuffer &) = delete;

    FrameBuffer(FrameBuffer &&frameBuffer) = default;

    GLuint getRenderTexture() const;

    void bindFrameBuffer() const;

    virtual void freeGPUMemory();

    virtual void clear() = 0;

    virtual ~FrameBuffer() = default;

    static void bindDefaultFrameBuffer(GLint defaultFrameBuffer);

    static void setViewportSize(GLsizei resolutionX, GLsizei resolutionY);

    static void enableDepthTest();

    static void disableDepthTest();

    static void enableBlending();

    static void disableBlending();

    static GLuint createTexture();

protected:
    static GLuint createFrameBufferObject();

private:
    const GLuint _fboHandle;
    const GLuint _renderTexture;
};

#endif /* FRAMEBUFFER_H */
