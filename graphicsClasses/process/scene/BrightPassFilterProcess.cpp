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
    _screenRenderPass(
        createBrightPassFilterProcessShaderProgram(fileContent),
        screen
    ),
    _brightPassFilterShader(_screenRenderPass.shaderProgram()),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        width,
        height,
        true,
        false
    )),
    _hdrSceneTextureSampler(
        TextureSampler::createInstance(
            hdrSceneTexture,
            0,
            _brightPassFilterShader,
            "textureScene"
        )) {
}

void BrightPassFilterProcess::render() const {
    FrameBuffer::disableDepthTest();
    _frameBuffer->bindFrameBuffer();
    FrameBuffer::setViewportSize(_width, _height);

    _brightPassFilterShader->use();
    _hdrSceneTextureSampler.bind();
    _screenRenderPass.render();
}

std::shared_ptr<const GLuint> BrightPassFilterProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

void BrightPassFilterProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _brightPassFilterShader->freeGPUMemory();
}

CstShaderProgram_sptr BrightPassFilterProcess::createBrightPassFilterProcessShaderProgram(
    const JBTypes::FileContent &fileContent
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "brightPassFilter.fs"
    );
    shader->use();
    return shader;
}

vecCstShaderProgram_sptr BrightPassFilterProcess::getShaderPrograms() const {
    return {_brightPassFilterShader};
}

