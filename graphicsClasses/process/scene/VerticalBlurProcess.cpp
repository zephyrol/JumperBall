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
    const CstRenderGroupsManager_sptr &screen
) :
    _verticalBlurShader(createVerticalBlurProcessShaderProgram(fileContent, width, height)),
    _screenRenderPass(_verticalBlurShader, screen),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        width,
        height,
        false,
        false
    )),
    _horizontalBlurTexture(horizontalBlurTexture) {
}

void VerticalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();

    _verticalBlurShader->use();
    TextureSampler::bind(_horizontalBlurTexture);

    _screenRenderPass.render();
}

std::shared_ptr<const GLuint> VerticalBlurProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void VerticalBlurProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _verticalBlurShader->freeGPUMemory();
}

ShaderProgram_sptr VerticalBlurProcess::createVerticalBlurProcessShaderProgram(
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
    shader->setTextureIndex("horizontalBlurTexture", 1);
    return shader;
}

vecCstShaderProgram_sptr VerticalBlurProcess::getShaderPrograms() const {
    return {_verticalBlurShader};
}

