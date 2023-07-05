//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "VerticalBlurProcess.h"

VerticalBlurProcess::VerticalBlurProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint horizontalBlurTexture,
    const CstRenderGroupsManager_sptr &screen
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
            texelSizeY * -12.f, 0.00110796,
            texelSizeY * -11.f, 0.00227339,
            texelSizeY * -10.f, 0.00438208,
            texelSizeY * -9.f, 0.00793491,
            texelSizeY * -8.f, 0.0134977,
            texelSizeY * -7.f, 0.0215693,
            texelSizeY * -6.f, 0.0323794,
            texelSizeY * -5.f, 0.0456623,
            texelSizeY * -4.f, 0.0604927,
            texelSizeY * -3.f, 0.0752844,
            texelSizeY * -2.f, 0.0880163,
            texelSizeY * -1.f, 0.096667,
            texelSizeY * 0.f, 0.0997356,
            texelSizeY * 1.f, 0.096667,
            texelSizeY * 2.f, 0.0880163,
            texelSizeY * 3.f, 0.0752844,
            texelSizeY * 4.f, 0.0604927,
            texelSizeY * 5.f, 0.0456623,
            texelSizeY * 6.f, 0.0323794,
            texelSizeY * 7.f, 0.0215693,
            texelSizeY * 8.f, 0.0134977,
            texelSizeY * 9.f, 0.00793491,
            texelSizeY * 10.f, 0.00438208,
            texelSizeY * 11.f, 0.00227339,
            texelSizeY * 12.f, 0.00110796
        }
    );


    return shader;
}

vecCstShaderProgram_sptr VerticalBlurProcess::getShaderPrograms() const {
    return {_verticalBlurShader};
}

