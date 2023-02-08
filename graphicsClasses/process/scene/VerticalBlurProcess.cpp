//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "VerticalBlurProcess.h"

#include <utility>

VerticalBlurProcess::VerticalBlurProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint horizontalBlurTexture,
    RenderPass_sptr screen
) :
    _screen(std::move(screen)),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        width,
        height,
        false,
        false
    )),
    _verticalBlurShader(createVerticalBlurProcessShaderProgram(fileContent, width, height)),
    _horizontalBlurTextureSampler(TextureSampler::createInstance(
        horizontalBlurTexture,
        1,
        _verticalBlurShader,
        "horizontalBlurTexture"
    )) {
}

void VerticalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();

    _verticalBlurShader->use();
    _horizontalBlurTextureSampler.bind();

    _screen->render();
}

std::shared_ptr<const GLuint> VerticalBlurProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void VerticalBlurProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _verticalBlurShader->freeGPUMemory();
}

CstShaderProgram_sptr VerticalBlurProcess::createVerticalBlurProcessShaderProgram(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "verticalBlurFs.fs",
        {},
        {{
             "texelSize", glm::vec2(
                1.f / static_cast<float>(width),
                1.f / static_cast<float>(height)
            )}
        }
    );
    shader->use();
    return shader;
}

vecCstShaderProgram_sptr VerticalBlurProcess::getShaderPrograms() const {
    return {_verticalBlurShader};
}

