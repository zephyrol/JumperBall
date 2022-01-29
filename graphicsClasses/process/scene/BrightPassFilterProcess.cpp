//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "BrightPassFilterProcess.h"

BrightPassFilterProcess::BrightPassFilterProcess(
        GLsizei width,
        GLsizei height,
        GLuint hdrSceneTexture,
        const RenderPass_sptr& screen
):
    _width(width),
    _height(height),
    _screen(screen),
    _frameBuffer(FrameBuffer_uptr(new FrameBuffer(
        width,
        height,
        FrameBuffer::Content::HDR,
        false,
        false
        ))
),
    _hdrSceneTexture(hdrSceneTexture),
    _brightPassFilterShader(createBrightPassFilterProcessShaderProgram())
{
}

void BrightPassFilterProcess::render() const {
    _frameBuffer->bindFrameBuffer();
    FrameBuffer::disableDepthTest();
    FrameBuffer::setViewportSize(_width, _height);

    _brightPassFilterShader->use();
    _screen->render(_brightPassFilterShader);
}

std::shared_ptr<const GLuint> BrightPassFilterProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void BrightPassFilterProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _brightPassFilterShader->freeGPUMemory();
}

CstShaderProgram_sptr BrightPassFilterProcess::createBrightPassFilterProcessShaderProgram() const {
    CstShaderProgram_sptr sp = ShaderProgram::createShaderProgram("basicFboVs.vs", "brightPassFilter.fs");
    sp->use();
    sp->bindUniformTexture("textureScene", 0, _hdrSceneTexture);
    return sp;
}

