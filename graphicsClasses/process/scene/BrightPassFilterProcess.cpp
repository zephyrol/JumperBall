//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "BrightPassFilterProcess.h"

#include <utility>

BrightPassFilterProcess::BrightPassFilterProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint hdrSceneTexture,
    const CstRenderGroupsManager_sptr &screen
) :
    _width(width),
    _height(height),
    _brightPassFilterShader(createBrightPassFilterProcessShaderProgram(fileContent)),
    _screenRenderPass(_brightPassFilterShader, screen),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        width,
        height,
        true,
        false
    )),
    _hdrSceneTexture(hdrSceneTexture) {
}

void BrightPassFilterProcess::render() const {
    FrameBuffer::disableDepthTest();
    FrameBuffer::disableBlending();
    _frameBuffer->bindFrameBuffer();
    FrameBuffer::setViewportSize(_width, _height);

    _brightPassFilterShader->use();
    TextureSampler::bind(_hdrSceneTexture);
    _screenRenderPass.render();
}

std::shared_ptr<const GLuint> BrightPassFilterProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void BrightPassFilterProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _brightPassFilterShader->freeGPUMemory();
}

ShaderProgram_sptr BrightPassFilterProcess::createBrightPassFilterProcessShaderProgram(
    const JBTypes::FileContent &fileContent
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "brightPassFilter.fs"
    );
    shader->use();
    shader->setTextureIndex("sceneTexture", 0);
    return shader;
}

vecCstShaderProgram_sptr BrightPassFilterProcess::getShaderPrograms() const {
    return {_brightPassFilterShader};
}
