//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "HorizontalBlurProcess.h"

HorizontalBlurProcess::HorizontalBlurProcess(
        const JBTypes::FileContent& fileContent,
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
        false
        ))
),
    _brightPassTexture(brightPassTexture),
    _horizontalBlurShader(createHorizontalBlurProcessShaderProgram(fileContent))
{
}

void HorizontalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();
    _horizontalBlurShader->use();

    ShaderProgram::setActiveTexture(1);
    ShaderProgram::bindTexture(_brightPassTexture);

    _screen->render(_horizontalBlurShader);
}

std::shared_ptr<const GLuint> HorizontalBlurProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void HorizontalBlurProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _horizontalBlurShader->freeGPUMemory();
}

CstShaderProgram_sptr HorizontalBlurProcess::createHorizontalBlurProcessShaderProgram(
    const JBTypes::FileContent& fileContent
) {
    auto shader = ShaderProgram::createShaderProgram(fileContent,"basicFboVs.vs", "horizontalBlurFs.fs");
    shader->use();
    shader->bindUniformTextureIndex("brightPassTexture", 1);
    return shader;
}

vecCstShaderProgram_sptr HorizontalBlurProcess::getShaderPrograms() const {
    return { _horizontalBlurShader };
}

