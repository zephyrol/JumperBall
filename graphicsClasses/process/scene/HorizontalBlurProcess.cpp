//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "HorizontalBlurProcess.h"

HorizontalBlurProcess::HorizontalBlurProcess(
        GLsizei width,
        GLsizei height,
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
        _bloomShader(ShaderProgram::createShaderProgram("basicFboVs.vs", "horizontalBlurFs.fs" ))
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

