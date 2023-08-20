//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "VerticalBlurProcess.h"

#include <utility>

VerticalBlurProcess::VerticalBlurProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint horizontalBlurTexture,
    const RenderGroup_sptr &screen
) :
    _verticalBlurShader(createVerticalBlurProcessShaderProgram(fileContent, height)),
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
    GLsizei height
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "verticalBlurFs.fs"
    );
    shader->use();
    shader->setTextureIndex("horizontalBlurTexture", 1);

    // Getting 25 Gauss weights computed with sigma = 4
    const auto texelSizeY = 1.f / static_cast<float>(height);
    shader->setUniformArrayVec2(
        "offsetsAndGaussWeights[0]",
        {
            texelSizeY * -12.f, 0.00110796f,
            texelSizeY * -11.f, 0.00227339f,
            texelSizeY * -10.f, 0.00438208f,
            texelSizeY * -9.f, 0.00793491f,
            texelSizeY * -8.f, 0.0134977f,
            texelSizeY * -7.f, 0.0215693f,
            texelSizeY * -6.f, 0.0323794f,
            texelSizeY * -5.f, 0.0456623f,
            texelSizeY * -4.f, 0.0604927f,
            texelSizeY * -3.f, 0.0752844f,
            texelSizeY * -2.f, 0.0880163f,
            texelSizeY * -1.f, 0.096667f,
            texelSizeY * 0.f, 0.0997356f,
            texelSizeY * 1.f, 0.096667f,
            texelSizeY * 2.f, 0.0880163f,
            texelSizeY * 3.f, 0.0752844f,
            texelSizeY * 4.f, 0.0604927f,
            texelSizeY * 5.f, 0.0456623f,
            texelSizeY * 6.f, 0.0323794f,
            texelSizeY * 7.f, 0.0215693f,
            texelSizeY * 8.f, 0.0134977f,
            texelSizeY * 9.f, 0.00793491f,
            texelSizeY * 10.f, 0.00438208f,
            texelSizeY * 11.f, 0.00227339f,
            texelSizeY * 12.f, 0.00110796f
        }
    );
    return shader;
}

vecCstShaderProgram_sptr VerticalBlurProcess::getShaderPrograms() const {
    return {_verticalBlurShader};
}

