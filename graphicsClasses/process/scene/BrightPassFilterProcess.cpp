//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "BrightPassFilterProcess.h"

BrightPassFilterProcess::BrightPassFilterProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint hdrSceneTexture,
    const RenderPass_sptr &screen
) :
    _width(width),
    _height(height),
    _screen(screen),
    _frameBuffer(FrameBuffer_uptr(new FrameBuffer(
                     width,
                     height,
                     FrameBuffer::Content::HDR,
                     false
                 ))
    ),
    _hdrSceneTexture(hdrSceneTexture),
    _brightPassFilterShader(createBrightPassFilterProcessShaderProgram(fileContent)) {
}

void BrightPassFilterProcess::render() const {
    FrameBuffer::disableDepthTest();
    _frameBuffer->bindFrameBuffer();
    FrameBuffer::setViewportSize(_width, _height);

    _brightPassFilterShader->use();
    ShaderProgram::bindTexture(_hdrSceneTexture);
    _screen->render(_brightPassFilterShader);
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
    auto shader = ShaderProgram::createShaderProgram(fileContent, "basicFboVs.vs", "brightPassFilter.fs");
    shader->use();
    shader->bindUniformTextureIndex("textureScene", 0);
    return shader;
}

vecCstShaderProgram_sptr BrightPassFilterProcess::getShaderPrograms() const {
    return {_brightPassFilterShader};
}

