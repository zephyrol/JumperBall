//
// Created by S.Morgenthaler on 21/08/2023.
//

#include "PostEffects.h"
#include "componentsGeneration/ScreenGroupGenerator.h"

PostEffects_uptr PostEffects::createInstance(const JBTypes::FileContent& fileContent,
                                             GLsizei screenWidth,
                                             GLsizei screenHeight,
                                             GLsizei postEffectsWidth,
                                             GLsizei postEffectsHeight,
                                             GLuint uniformBufferBindingPoint,
                                             const std::string& uniformBufferName,
                                             const CstTextureSampler_uptr& sceneTexture,
                                             GLint defaultFrameBuffer,
                                             RenderingCache& renderingCache) {
    auto brightPassFilterFrameBuffer =
        ColorableFrameBuffer::createInstance(postEffectsWidth, postEffectsHeight, true, false);
    auto horizontalBlurFrameBuffer(
        ColorableFrameBuffer::createInstance(postEffectsWidth, postEffectsHeight, true, false));
    auto verticalBlurFrameBuffer(
        ColorableFrameBuffer::createInstance(postEffectsWidth, postEffectsHeight, false, false));

    std::vector<FrameBuffer_uptr> frameBuffers;
    for (const auto& nameNearestFilter : {std::pair<std::string, bool>{brightPassFilterFrameBufferHashBase, true},
                                          std::pair<std::string, bool>{horizontalBlurFrameBufferHashBase, true},
                                          std::pair<std::string, bool>{verticalBlurFrameBufferHashBase, false}}) {
        auto frameBuffer = renderingCache.getFrameBuffer(
            getFrameBufferHash(nameNearestFilter.first, postEffectsWidth, postEffectsHeight));
        if (frameBuffer == nullptr) {
            frameBuffer = ColorableFrameBuffer::createInstance(postEffectsWidth, postEffectsHeight,
                                                               nameNearestFilter.second, false);
        }
        frameBuffers.emplace_back(std::move(frameBuffer));
    }

    auto postProcessesShader = createPostProcessesShaderProgram(
        sceneTexture, fileContent, postEffectsWidth, postEffectsHeight, uniformBufferBindingPoint,
        uniformBufferName, verticalBlurFrameBuffer, renderingCache);

    return PostEffects_uptr(new PostEffects(
        screenWidth, screenHeight, postEffectsWidth, postEffectsHeight,
        std::move(brightPassFilterFrameBuffer), std::move(horizontalBlurFrameBuffer),
        std::move(verticalBlurFrameBuffer), std::move(postProcessesShader), defaultFrameBuffer));
}

PostEffects::PostEffects(GLsizei screenWidth,
                         GLsizei screenHeight,
                         GLsizei postEffectsWidth,
                         GLsizei postEffectsHeight,
                         FrameBuffer_uptr brightPassFilterFrameBuffer,
                         FrameBuffer_uptr horizontalBlurFrameBuffer,
                         FrameBuffer_uptr verticalBlurFrameBuffer,
                         ShaderProgram_uptr postProcessesShader,
                         GLint defaultFrameBuffer)
    : _screenWidth(screenWidth),
      _screenHeight(screenHeight),
      _postEffectsWidth(postEffectsWidth),
      _postEffectsHeight(postEffectsHeight),
      _screen([]() {
          ScreenGroupGenerator screenGroupGenerator;
          return screenGroupGenerator.genRenderGroup();
      }()),
      _brightPassFilterFrameBuffer(std::move(brightPassFilterFrameBuffer)),
      _horizontalBlurFrameBuffer(std::move(horizontalBlurFrameBuffer)),
      _verticalBlurFrameBuffer(std::move(verticalBlurFrameBuffer)),
      _postProcessesShader(std::move(postProcessesShader)),
      _postProcessIdUniformLocation(_postProcessesShader->getUniformLocation("postProcessId")),
      _defaultFrameBuffer(defaultFrameBuffer) {}

void PostEffects::render() const {
    FrameBuffer::disableDepthTest();
    TextureSampler::setActiveTexture(postProcessTextureNumber);

    _postProcessesShader->use();
    // 1. Bright pass filter
    _brightPassFilterFrameBuffer->bindFrameBuffer();
    FrameBuffer::setViewportSize(_postEffectsWidth, _postEffectsHeight);

    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 0);
    _screen->bind();
    _screen->render();

    // 2. Horizontal blur
    _horizontalBlurFrameBuffer->bindFrameBuffer();
    _brightPassFilterFrameBuffer->getRenderTexture()->bind();
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 1);
    _screen->render();

    // 3. Vertical blur
    _verticalBlurFrameBuffer->bindFrameBuffer();
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 2);
    _horizontalBlurFrameBuffer->getRenderTexture()->bind();
    _screen->render();

    // 4. Bloom
    GpuFrameBuffer::bindDefaultFrameBuffer(_defaultFrameBuffer);
    _postProcessesShader->setInteger(_postProcessIdUniformLocation, 3);
    FrameBuffer::setViewportSize(_screenWidth, _screenHeight);

    _verticalBlurFrameBuffer->getRenderTexture()->bind();
    _screen->render();
}

std::string PostEffects::getFrameBufferHash(const std::string& base, GLsizei width, GLsizei height) {
    return base + std::to_string(width) + ";" + std::to_string(height);
}

ShaderProgram_uptr PostEffects::createPostProcessesShaderProgram(
    const CstTextureSampler_uptr& sceneTexture,
    const JBTypes::FileContent& fileContent,
    GLsizei width,
    GLsizei height,
    GLuint uniformBufferBindingPoint,
    const std::string& uniformBufferName,
    ColorableFrameBuffer_uptr const& verticalBlurFrameBuffer,
    RenderingCache& renderingCache) {
    const std::string shaderHash = "postEffects";
    auto shader = renderingCache.getShaderProgram(shaderHash);
    if (shader == nullptr) {
        shader =
            ShaderProgram::createInstance(fileContent, "postEffectsVs.vs", "postEffectsFs.fs", shaderHash, {},
                                          {}, {}, {}, {{uniformBufferName, uniformBufferBindingPoint}});
    }
    shader->use();

    constexpr GLint sceneTextureNumber = 2;
    shader->setTextureIndex("sceneTexture", sceneTextureNumber);
    TextureSampler::setActiveTexture(sceneTextureNumber);
    sceneTexture->bind();

    shader->setTextureIndex("postProcessTexture", postProcessTextureNumber);
    TextureSampler::setActiveTexture(postProcessTextureNumber);
    verticalBlurFrameBuffer->getRenderTexture()->bind();
    // Getting 17 Gauss weights computed with sigma = 4. Because two standard deviations from mean account
    // for 95.45%
    const auto texelSizeX = 1.f / static_cast<float>(width);
    const auto texelSizeY = 1.f / static_cast<float>(height);

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
    return shader;
}
void PostEffects::fillCache(RenderingCache& renderingCache) {
    renderingCache.setFrameBuffer(getFrameBufferHash(brightPassFilterFrameBufferHashBase, _screenWidth, _screenHeight), std::move(_brightPassFilterFrameBuffer));
    renderingCache.setFrameBuffer(getFrameBufferHash(horizontalBlurFrameBufferHashBase, _screenWidth, _screenHeight), std::move(_brightPassFilterFrameBuffer));
    renderingCache.setFrameBuffer(getFrameBufferHash(verticalBlurFrameBufferHashBase, _screenWidth, _screenHeight), std::move(_brightPassFilterFrameBuffer));
}

const std::string PostEffects::brightPassFilterFrameBufferHashBase = "brightPassFilter";
const std::string PostEffects::horizontalBlurFrameBufferHashBase = "horizontalBlur";
const std::string PostEffects::verticalBlurFrameBufferHashBase = "verticalBlur";
