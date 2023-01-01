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
void clean();

void freeGPUMemory();

static void bindDefaultFrameBuffer(GLint defaultFrameBuffer);
static void cleanDefaultFrameBuffer();
static void setViewportSize(GLsizei resolutionX, GLsizei resolutionY);
static void enableDepthTest();
static void disableDepthTest();

    static GLuint createTexture() ;

private:

const GLuint _fboHandle;

const GLuint _renderTexture;
const Content _content;

const std::unique_ptr <const GLuint> _depthBuffer;

const glm::vec3 _clearColor;

static GLuint createFrameBufferObject() ;

    static std::unique_ptr <GLuint> createDepthBuffer() ;
bool hasDepthBuffer() const;
static void cleanCurrentFrameBuffer(bool hasDepthBuffer, const glm::vec3& clearColor);

};

#endif /* FRAMEBUFFER_H */
