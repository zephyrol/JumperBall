//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "BloomProcess.h"

BloomProcess::BloomProcess(
    GLsizei width,
    GLsizei height,
    GLuint sceneHDRTexture,
    GLuint bluredTexture,
    const RenderPass_sptr& screen
):
        _width(width),
        _height(height),
        _screen(screen),
        _sceneHDRTexture(sceneHDRTexture),
        _bluredTexture(bluredTexture),
        _bloomShader(createBloomProcessShaderProgram())
{
}

void BloomProcess::render() const {
    FrameBuffer::bindDefaultFrameBuffer();
    FrameBuffer::setViewportSize(_width, _height);
    //FrameBuffer::cleanDefaultFrameBuffer();
    _bloomShader->use();
    _bloomShader->bindUniformTexture("frameSceneHDRTexture", 0, _sceneHDRTexture);
    _bloomShader->bindUniformTexture("frameBluredTexture", 1, _bluredTexture);
    _screen->render(_bloomShader);
}

void BloomProcess::freeGPUMemory() {
    _bloomShader->freeGPUMemory();
}

std::shared_ptr<const GLuint> BloomProcess::getRenderTexture() const {
    return nullptr;
}

CstShaderProgram_sptr BloomProcess::createBloomProcessShaderProgram() {
    return ShaderProgram::createShaderProgram("basicFboVs.vs", "bloomFs.fs");
}

vecCstShaderProgram_sptr BloomProcess::getShaderPrograms() const {
    return { _bloomShader };
}

