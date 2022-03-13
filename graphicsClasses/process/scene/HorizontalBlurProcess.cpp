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
    _horizontalBlurShader(createHorizontalBlurProcessShaderProgram(fileContent, width, height))
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
    const JBTypes::FileContent& fileContent,
    GLsizei width,
    GLsizei height
) {
    auto shader = ShaderProgram::createShaderProgram(
        fileContent,
        "basicFboVs.vs",
        "horizontalBlurFs.fs",
        {},
        {{"texelSize", glm::vec2(
            1.f / static_cast<float>(width),
            1.f / static_cast<float>(height)
        )}}
    );
    shader->use();
    shader->bindUniformTextureIndex("brightPassTexture", 1);
    return shader;
}

vecCstShaderProgram_sptr HorizontalBlurProcess::getShaderPrograms() const {
    return { _horizontalBlurShader };
}

