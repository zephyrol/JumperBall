//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "VerticalBlurProcess.h"

VerticalBlurProcess::VerticalBlurProcess(
        const JBTypes::FileContent& fileContent,
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
        false
    ))
),
    _horizontalBlurTexture(horizontalBlurTexture),
    _verticalBlurShader(createVerticalBlurProcessShaderProgram(fileContent))
{
}

void VerticalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();

    _verticalBlurShader->use();
    ShaderProgram::bindTexture(_horizontalBlurTexture);

    _screen->render(_verticalBlurShader);
}

std::shared_ptr<const GLuint> VerticalBlurProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void VerticalBlurProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _verticalBlurShader->freeGPUMemory();
}

CstShaderProgram_sptr VerticalBlurProcess::createVerticalBlurProcessShaderProgram(
    const JBTypes::FileContent& fileContent
    ) {
    auto shader = ShaderProgram::createShaderProgram(fileContent, "basicFboVs.vs", "verticalBlurFs.fs");
    shader->use();
    shader->bindUniformTextureIndex("horizontalBlurTexture", 1);
    return shader;
}

vecCstShaderProgram_sptr VerticalBlurProcess::getShaderPrograms() const {
    return {_verticalBlurShader };
}

