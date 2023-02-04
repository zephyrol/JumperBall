//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "HorizontalBlurProcess.h"
#include "frameBuffer/TextureSampler.h"

#include <utility>

HorizontalBlurProcess::HorizontalBlurProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint brightPassTexture,
    const RenderGroupsManager_sptr& screen
) :
    _screenRenderPass(
        createHorizontalBlurProcessShaderProgram(fileContent, width, height),
        screen
    ),
    _horizontalBlurShader(_screenRenderPass.shaderProgram()),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        width,
        height,
        true,
        false
    )),
    _brightPassTexture(brightPassTexture)

}

void HorizontalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();
    _horizontalBlurShader->use();

    TextureSampler::setActiveTexture(1);
    ShaderProgram::bindTexture(_brightPassTexture);
    _screenRenderPass.render();
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

