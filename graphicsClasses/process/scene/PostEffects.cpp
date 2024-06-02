//
// Created by S.Morgenthaler on 21/08/2023.
//

#include "PostEffects.h"
#include "componentsGeneration/ScreenGroupGenerator.h"

PostEffects::PostEffects(GLsizei screenWidth,
                         GLsizei screenHeight,
                         GLsizei postEffectsWidth,
                         GLsizei postEffectsHeight,
                         RenderGroup_sptr screen,
                         ColorableFrameBuffer_uptr brightPassFilterFrameBuffer,
                         ColorableFrameBuffer_uptr horizontalBlurFrameBuffer,
                         ColorableFrameBuffer_uptr verticalBlurFrameBuffer,
                         ShaderProgram_sptr postProcessesShader,
                         GLint postProcessIdUniformLocation,
                         GLint postProcessTextureUniformLocation,
                         GLint defaultFrameBuffer)
    : _screenWidth(screenWidth),
      _screenHeight(screenHeight),
      _postEffectsWidth(postEffectsWidth),
      _postEffectsHeight(postEffectsHeight),
      _screen(std::move(screen)),
      _brightPassFilterFrameBuffer(std::move(brightPassFilterFrameBuffer)),
      _horizontalBlurFrameBuffer(std::move(horizontalBlurFrameBuffer)),
      _verticalBlurFrameBuffer(std::move(verticalBlurFrameBuffer)),
      _postProcessesShader(std::move(postProcessesShader)),
      _postProcessIdUniformLocation(postProcessIdUniformLocation),
      _postProcessTextureUniformLocation(postProcessTextureUniformLocation),
      _defaultFrameBuffer(defaultFrameBuffer) {}

PostEffects_uptr PostEffects::createInstance(const JBTypes::FileContent& fileContent,
                                             GLsizei screenWidth,
                                             GLsizei screenHeight,
                                             GLsizei postEffectsWidth,
                                             GLsizei postEffectsHeight,
                                             GLint sceneTextureIndex,
                                             GLint defaultFrameBuffer) {
    auto screen = ([]() {
        ScreenGroupGenerator screenGroupGenerator;
        return screenGroupGenerator.genRenderGroup();
    }());

    TextureSampler::setActiveTexture(brightPassFilterTextureIndex);
    auto brightPassFilterFrameBuffer =
        ColorableFrameBuffer::createInstance(postEffectsWidth, postEffectsHeight, true, false);

    TextureSampler::setActiveTexture(horizontalBlurTextureIndex);
    auto horizontalBlurFrameBuffer =
        ColorableFrameBuffer::createInstance(postEffectsWidth, postEffectsHeight, true, false);

    TextureSampler::setActiveTexture(verticalBlurTextureIndex);
    auto verticalBlurFrameBuffer =
        ColorableFrameBuffer::createInstance(postEffectsWidth, postEffectsHeight, false, false);

    auto shader = ShaderProgram::createInstance(fileContent, "postEffectsVs.vs", "postEffectsFs.fs");
    shader->use();
    shader->setTextureIndex("sceneTexture", sceneTextureIndex);

    // shader->setTextureIndex("postProcessTexture", postProcessTextureNumber);
    // Getting 17 Gauss weights computed with sigma = 4. Because two standard deviations from mean account

    // for 95.45%
    const auto texelSizeX = 1.f / static_cast<float>(postEffectsWidth);
    const auto texelSizeY = 1.f / static_cast<float>(postEffectsHeight);

    shader->setUniformArrayVec2(
        "offsetsAndGaussWeights[0]",
        {
            texelSizeX * -8.f, 0.0134977f, texelSizeX * -7.f, 0.0215693f, texelSizeX * -6.f, 0.0323794f,
            texelSizeX * -5.f, 0.0456623f, texelSizeX * -4.f, 0.0604927f, texelSizeX * -3.f, 0.0752844f,
            texelSizeX * -2.f, 0.0880163f, texelSizeX * -1.f, 0.096667f,  texelSizeX * 0.f,  0.0997356f,
            texelSizeX * 1.f,  0.096667f,  texelSizeX * 2.f,  0.0880163f, texelSizeX * 3.f,  0.0752844f,
            texelSizeX * 4.f,  0.0604927f, texelSizeX * 5.f,  0.0456623f, texelSizeX * 6.f,  0.0323794f,
            texelSizeX * 7.f,  0.0215693f, texelSizeX * 8.f,  0.0134977f, texelSizeY * -8.f, 0.0134977f,
            texelSizeY * -7.f, 0.0215693f, texelSizeY * -6.f, 0.0323794f, texelSizeY * -5.f, 0.0456623f,
            texelSizeY * -4.f, 0.0604927f, texelSizeY * -3.f, 0.0752844f, texelSizeY * -2.f, 0.0880163f,
            texelSizeY * -1.f, 0.096667f,  texelSizeY * 0.f,  0.0997356f, texelSizeY * 1.f,  0.096667f,
            texelSizeY * 2.f,  0.0880163f, texelSizeY * 3.f,  0.0752844f, texelSizeY * 4.f,  0.0604927f,
            texelSizeY * 5.f,  0.0456623f, texelSizeY * 6.f,  0.0323794f, texelSizeY * 7.f,  0.0215693f,
            texelSizeY * 8.f,  0.0134977f,
        });

    const auto postProcessIdUniformLocation = shader->getUniformLocation("postProcessId");
    const auto postProcessTextureUniformLocation = shader->getUniformLocation("postProcessTexture");

    // Setting the current texture to vertical blur to keep consistency during the beginning of rendering.
    // (common state at the end of rendering).
    shader->setInteger(postProcessTextureUniformLocation, verticalBlurTextureIndex);

    return PostEffects_uptr(
        new PostEffects(screenWidth, screenHeight, postEffectsWidth, postEffectsHeight, std::move(screen),
                        std::move(brightPassFilterFrameBuffer), std::move(horizontalBlurFrameBuffer),
                        std::move(verticalBlurFrameBuffer), std::move(shader), postProcessIdUniformLocation,
                        postProcessTextureUniformLocation, defaultFrameBuffer));
}

void PostEffects::render() const {
    FrameBuffer::disableDepthTest();
    _postProcessesShader->use();

    // 1. Bright pass filter
    _brightPassFilterFrameBuffer->bindFrameBuffer();
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 0);
    _screen->bind();
    FrameBuffer::setViewportSize(_postEffectsWidth, _postEffectsHeight);
    _screen->render();

    // 2. Horizontal blur
    _horizontalBlurFrameBuffer->bindFrameBuffer();
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 1);
    _postProcessesShader->setInteger(_postProcessTextureUniformLocation, brightPassFilterTextureIndex);
    _screen->render();

    // 3. Vertical blur
    _verticalBlurFrameBuffer->bindFrameBuffer();
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 2);
    _postProcessesShader->setInteger(_postProcessTextureUniformLocation, horizontalBlurTextureIndex);
    _screen->render();

    // 4. Bloom
    GpuFrameBuffer::bindDefaultFrameBuffer(_defaultFrameBuffer);
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 3);
    _postProcessesShader->setInteger(_postProcessTextureUniformLocation, verticalBlurTextureIndex);
    FrameBuffer::setViewportSize(_screenWidth, _screenHeight);
    _screen->render();
}

vecCstShaderProgram_sptr PostEffects::getShaderPrograms() const {
    return {_postProcessesShader};
}
