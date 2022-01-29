//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "VerticalBlurProcess.h"

VerticalBlurProcess::VerticalBlurProcess(
        GLsizei width,
        GLsizei height,
        GLuint horizontalBlurTexture,
        const RenderPass_sptr& screen
):
        _screen(screen),
        _frameBuffer(FrameBuffer_uptr(new FrameBuffer(
        width,
        height,
        FrameBuffer::Content::SDR,
        false,
        false ))
),
        _horizontalBlurTexture(horizontalBlurTexture),
        _bloomShader(createVerticalBlurProcessShaderProgram())
{
}

void VerticalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();

    _bloomShader->use();
    _screen->render(_bloomShader);
}

std::shared_ptr<const GLuint> VerticalBlurProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void VerticalBlurProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _bloomShader->freeGPUMemory();
}

CstShaderProgram_sptr VerticalBlurProcess::createVerticalBlurProcessShaderProgram() const {
    CstShaderProgram_sptr sp = ShaderProgram::createShaderProgram("basicFboVs.vs", "verticalBlurFs.fs");
    sp->use();
    sp->bindUniformTexture("brightPassTexture", 0, _horizontalBlurTexture);
    return sp;
}

