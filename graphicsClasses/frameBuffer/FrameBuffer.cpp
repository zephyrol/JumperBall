/*
 * File: FrameBuffer.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(
    CstTextureSampler_uptr renderTexture,
    CstRenderBuffer_uptr renderBuffer,
    CstGpuFrameBuffer_uptr gpuFrameBuffer
):
    _renderTexture(std::move(renderTexture)),
    _renderBuffer(std::move(renderBuffer)),
    _gpuFrameBuffer(std::move(gpuFrameBuffer)) {
}

void FrameBuffer::bindFrameBuffer() const {
    _gpuFrameBuffer->bind();
}

const CstTextureSampler_uptr &FrameBuffer::getRenderTexture() const {
    return _renderTexture;
}

void FrameBuffer::setViewportSize(GLsizei resolutionX, GLsizei resolutionY) {
    glViewport(0, 0, resolutionX, resolutionY);
}

void FrameBuffer::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::enableBlending() {
    glEnable(GL_BLEND);
}

void FrameBuffer::disableBlending() {
    glDisable(GL_BLEND);
}
