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

public:

enum class TextureCaterory { SDR, HDR, Depth };

FrameBuffer(TextureCaterory category = TextureCaterory::SDR,
            GLsizei resolutionX = Utility::windowResolutionX,
            GLsizei resolutionY = Utility::windowResolutionY,
            bool hasDepthBuffer = true);

FrameBuffer(const FrameBuffer& frameBuffer) = delete;
FrameBuffer& operator= (const FrameBuffer&) = delete;

~FrameBuffer() = default;

static constexpr float luminanceKey = 0.4f;

GLuint getHandle() const;
void bindFrameBuffer(bool clean = true) const;
std::pair <float, float> computeLogAverageLuminanceAndMax() const;
GLuint getRenderTexture() const;


static void bindDefaultFrameBuffer(bool clean = true);
static void cleanCurrentFrameBuffer(bool hasDepthBuffer = true);

private:

GLuint _fboHandle;

GLuint _renderTexture;
const TextureCaterory _textureCategory;

const bool _hasDepthBuffer;
GLuint _depthBuffer;

const GLsizei _resolutionX;
const GLsizei _resolutionY;
};

#endif /* FRAMEBUFFER_H */
