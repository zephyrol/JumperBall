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
using FrameBuffer_uptr = std::unique_ptr <FrameBuffer>;
using CstFrameBuffer_uptr = std::unique_ptr <const FrameBuffer>;

class FrameBuffer {

private:
static const glm::vec3 backgroundColor;

public:
enum class Content { SDR, HDR, Depth };

FrameBuffer(
    GLsizei resolutionX,
    GLsizei resolutionY,
    Content content = Content::SDR,
    // TODO: remove auto clean, force to set clear color and scene rendering call clearMethod
    bool usedAutoClean = false,
    bool hasDepthBuffer = true,
    const glm::vec3& clearColor = backgroundColor
);

FrameBuffer(const FrameBuffer& frameBuffer) = delete;
FrameBuffer& operator= (const FrameBuffer&) = delete;

~FrameBuffer() = default;

static constexpr float luminanceKey = 0.4f;

GLuint getHandle() const;
void bindFrameBuffer() const;
GLuint getRenderTexture() const;

void freeGPUMemory();

static void bindDefaultFrameBuffer(GLsizei width, GLsizei height);
static void cleanDefaultFrameBuffer();
static FrameBuffer_uptr createScreenSpaceEffectFrameBuffer(
    const FrameBuffer::Content& category,
    GLsizei width,
    GLsizei height
);


private:

const GLuint _fboHandle;

const GLuint _renderTexture;
const Content _content;

const std::unique_ptr <const GLuint> _depthBuffer;

const bool _usedAutoClean;
const glm::vec3 _clearColor;

const GLsizei _resolutionX;
const GLsizei _resolutionY;

GLuint createFrameBufferObject() const;
GLuint createRenderTexture() const;
std::unique_ptr <GLuint> createDepthBuffer() const;
bool hasDepthBuffer() const;
static void cleanCurrentFrameBuffer(bool hasDepthBuffer, const glm::vec3& clearColor);

};

#endif /* FRAMEBUFFER_H */
