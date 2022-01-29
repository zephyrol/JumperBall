//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "HorizontalBlurProcess.h"

HorizontalBlurProcess::HorizontalBlurProcess(
        GLsizei width,
        GLsizei height,
        GLuint brightPassTexture,
        const RenderPass_sptr& screen
):
        _screen(screen),
        _frameBuffer(FrameBuffer_uptr(new FrameBuffer(
        width,
        height,
        FrameBuffer::Content::HDR,
        false,
        false
        ))
),
_brightPassTexture(brightPassTexture),
        _bloomShader(createHorizontalBlurProcessShaderProgram())
{
}

void HorizontalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();
    _bloomShader->use();
    _screen->render(_bloomShader);
}

std::shared_ptr<const GLuint> HorizontalBlurProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void HorizontalBlurProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _bloomShader->freeGPUMemory();
}

CstShaderProgram_sptr HorizontalBlurProcess::createHorizontalBlurProcessShaderProgram() const {
    CstShaderProgram_sptr sp = ShaderProgram::createShaderProgram("basicFboVs.vs", "horizontalBlurFs.fs");
    sp->use();
    sp->bindUniformTexture("brightPassTexture", 0, _brightPassTexture);
    return sp;
}

