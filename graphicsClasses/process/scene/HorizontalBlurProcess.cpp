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
    const RenderGroup_sptr &screen
) :
    _horizontalBlurShader(createHorizontalBlurProcessShaderProgram(fileContent, width)),
    _screenRenderPass(
        _horizontalBlurShader,
        screen
    ),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        width,
        height,
        true,
        false
    )),
    _brightPassTexture(brightPassTexture) {
}

void HorizontalBlurProcess::render() const {
    _frameBuffer->bindFrameBuffer();
    _horizontalBlurShader->use();

    TextureSampler::setActiveTexture(1);
    TextureSampler::bind(_brightPassTexture);
    _screenRenderPass.render();
}

std::shared_ptr<const GLuint> HorizontalBlurProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void HorizontalBlurProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _horizontalBlurShader->freeGPUMemory();
}

ShaderProgram_sptr HorizontalBlurProcess::createHorizontalBlurProcessShaderProgram(
    const JBTypes::FileContent &fileContent,
    GLsizei width
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "horizontalBlurFs.fs"
    );
    shader->use();
    shader->setTextureIndex("brightPassTexture", 1);

    // Getting 25 Gauss weights computed with sigma = 4
    const auto texelSizeX = 1.f / static_cast<float>(width);
    shader->setUniformArrayVec2(
        "offsetsAndGaussWeights[0]",
        {
            texelSizeX * -12.f, 0.00110796,
            texelSizeX * -11.f, 0.00227339,
            texelSizeX * -10.f, 0.00438208,
            texelSizeX * -9.f, 0.00793491,
            texelSizeX * -8.f, 0.0134977,
            texelSizeX * -7.f, 0.0215693,
            texelSizeX * -6.f, 0.0323794,
            texelSizeX * -5.f, 0.0456623,
            texelSizeX * -4.f, 0.0604927,
            texelSizeX * -3.f, 0.0752844,
            texelSizeX * -2.f, 0.0880163,
            texelSizeX * -1.f, 0.096667,
            texelSizeX * 0.f, 0.0997356,
            texelSizeX * 1.f, 0.096667,
            texelSizeX * 2.f, 0.0880163,
            texelSizeX * 3.f, 0.0752844,
            texelSizeX * 4.f, 0.0604927,
            texelSizeX * 5.f, 0.0456623,
            texelSizeX * 6.f, 0.0323794,
            texelSizeX * 7.f, 0.0215693,
            texelSizeX * 8.f, 0.0134977,
            texelSizeX * 9.f, 0.00793491,
            texelSizeX * 10.f, 0.00438208,
            texelSizeX * 11.f, 0.00227339,
            texelSizeX * 12.f, 0.00110796
        }
    );

    return shader;
}

vecCstShaderProgram_sptr HorizontalBlurProcess::getShaderPrograms() const {
    return {_horizontalBlurShader};
}
