//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "HorizontalBlurProcess.h"

#include <utility>

HorizontalBlurProcess::HorizontalBlurProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint brightPassTexture,
    RenderPass_sptr screen
) :
    _screen(std::move(screen)),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        width,
        height,
        true,
        false
    )),
    _brightPassTexture(brightPassTexture),
    _horizontalBlurShader(createHorizontalBlurProcessShaderProgram(fileContent, width, height)) {
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
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height
) {
    constexpr auto brightPassTextureUniformName = "brightPassTexture";
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "horizontalBlurFs.fs",
        {brightPassTextureUniformName},
        {},
        {{"texelSize", glm::vec2(
            1.f / static_cast<float>(width),
            1.f / static_cast<float>(height)
        )}}
    );
    shader->use();
    shader->bindUniformTextureIndex(brightPassTextureUniformName, 1);
    return shader;
}

vecCstShaderProgram_sptr HorizontalBlurProcess::getShaderPrograms() const {
    return {_horizontalBlurShader};
}

