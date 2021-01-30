/*
 * File: FrameBuffer.h
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "ShaderProgram.h"

class FrameBuffer {

public:

// ------------TYPES------------//
enum class TextureCaterory { SDR, HDR, Depth };

// --CONSTRUCTORS & DESTRUCTORS--//
FrameBuffer(TextureCaterory category = TextureCaterory::SDR,
            GLsizei resolutionX = Utility::windowResolutionX,
            GLsizei resolutionY = Utility::windowResolutionY,
            bool hasDepthBuffer = true);

// ---------CONSTANTS------------//
static constexpr float luminanceKey = 0.4f;

// -------CONST METHODS----------//
GLuint getHandle() const;
void bindFrameBuffer(bool clean = true) const;
std::pair <float, float> computeLogAverageLuminanceAndMax() const;
GLuint getRenderTexture() const;


// --------STATIC METHODS-------//
static void bindDefaultFrameBuffer(bool clean = true);
static void cleanCurrentFrameBuffer(bool hasDepthBuffer = true);

private:

// --------ATTRIBUTES-----------//
GLuint _fboHandle;

GLuint _renderTexture;
const TextureCaterory _textureCategory;

const bool _hasDepthBuffer;
GLuint _depthBuffer;

const GLsizei _resolutionX;
const GLsizei _resolutionY;
};

#endif /* FRAMEBUFFER_H */
