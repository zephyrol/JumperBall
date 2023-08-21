//
// Created by S.Morgenthaler on 21/08/2023.
//

#include "PostEffects.h"

PostEffects::PostEffects(
    const JBTypes::FileContent &fileContent,
    GLsizei screenWidth,
    GLsizei screenHeight,
    GLsizei postEffectsWidth,
    GLsizei postEffectsHeight,
    GLuint sceneTexture,
    GLint defaultFrameBuffer,
    CstRenderGroup_sptr screen
) :
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    _postEffectsWidth(postEffectsWidth),
    _postEffectsHeight(postEffectsHeight),
    _postProcessesShader(createPostProcessesShaderProgram(fileContent, postEffectsWidth, postEffectsHeight)),
    _postProcessIdUniformLocation(_postProcessesShader->getUniformLocation("postProcessId")),
    _screen(std::move(screen)),
    _brightPassFilterFrameBuffer(ColorableFrameBuffer::createInstance(
        postEffectsWidth,
        postEffectsHeight,
        true,
        false
    )),
    _horizontalBlurFrameBuffer(ColorableFrameBuffer::createInstance(
        postEffectsWidth,
        postEffectsHeight,
        true,
        false
    )),
    _verticalBlurFrameBuffer(ColorableFrameBuffer::createInstance(
        postEffectsWidth,
        postEffectsHeight,
        false,
        false
    )),
    _sceneTexture(sceneTexture),
    _defaultFrameBuffer(defaultFrameBuffer) {
}

void PostEffects::render() const {

    _postProcessesShader->use();

    FrameBuffer::disableDepthTest();
    FrameBuffer::disableBlending();

    // 1. Bright pass filter
    _brightPassFilterFrameBuffer->bindFrameBuffer();
    FrameBuffer::setViewportSize(_postEffectsWidth, _postEffectsHeight);

    TextureSampler::bind(_sceneTexture);

    // TODO : render several time but only one bind vao !
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 0);
    _screen->render();

    // 2. Horizontal blur
    _horizontalBlurFrameBuffer->bindFrameBuffer();
    TextureSampler::setActiveTexture(1);
    TextureSampler::bind(_brightPassFilterFrameBuffer->getRenderTexture());
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 1);
    _screen->render();

    // 3. Vertical blur
    _verticalBlurFrameBuffer->bindFrameBuffer();
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 2);
    TextureSampler::bind(_horizontalBlurFrameBuffer->getRenderTexture());
    _screen->render();

    // 4. Bloom
    FrameBuffer::bindDefaultFrameBuffer(_defaultFrameBuffer);
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 3);
    FrameBuffer::setViewportSize(_screenWidth, _screenHeight);

    TextureSampler::bind(_verticalBlurFrameBuffer->getRenderTexture());
    _screen->render();
}

void PostEffects::freeGPUMemory() {
    _brightPassFilterFrameBuffer->freeGPUMemory();
    _horizontalBlurFrameBuffer->freeGPUMemory();
    _verticalBlurFrameBuffer->freeGPUMemory();
    _postProcessesShader->freeGPUMemory();
}

ShaderProgram_sptr PostEffects::createPostProcessesShaderProgram(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "postEffectsFs.fs"
    );
    shader->use();
    shader->setTextureIndex("sceneTexture", 0);
    shader->setTextureIndex("postProcessTexture", 1);

    // Getting 25 Gauss weights computed with sigma = 4
    const auto texelSizeX = 1.f / static_cast<float>(width);
    const auto texelSizeY = 1.f / static_cast<float>(height);

    shader->setUniformArrayVec2(
        "offsetsAndGaussWeights[0]",
        {
            texelSizeX * -12.f, 0.00110796f,
            texelSizeX * -11.f, 0.00227339f,
            texelSizeX * -10.f, 0.00438208f,
            texelSizeX * -9.f, 0.00793491f,
            texelSizeX * -8.f, 0.0134977f,
            texelSizeX * -7.f, 0.0215693f,
            texelSizeX * -6.f, 0.0323794f,
            texelSizeX * -5.f, 0.0456623f,
            texelSizeX * -4.f, 0.0604927f,
            texelSizeX * -3.f, 0.0752844f,
            texelSizeX * -2.f, 0.0880163f,
            texelSizeX * -1.f, 0.096667f,
            texelSizeX * 0.f, 0.0997356f,
            texelSizeX * 1.f, 0.096667f,
            texelSizeX * 2.f, 0.0880163f,
            texelSizeX * 3.f, 0.0752844f,
            texelSizeX * 4.f, 0.0604927f,
            texelSizeX * 5.f, 0.0456623f,
            texelSizeX * 6.f, 0.0323794f,
            texelSizeX * 7.f, 0.0215693f,
            texelSizeX * 8.f, 0.0134977f,
            texelSizeX * 9.f, 0.00793491f,
            texelSizeX * 10.f, 0.00438208f,
            texelSizeX * 11.f, 0.00227339f,
            texelSizeX * 12.f, 0.00110796f,
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

vecCstShaderProgram_sptr PostEffects::getShaderPrograms() const {
    return {_postProcessesShader};
}

std::shared_ptr<const GLuint> PostEffects::getRenderTexture() const {
    return nullptr;
}
