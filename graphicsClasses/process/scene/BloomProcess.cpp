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
    FrameBuffer::setViewportSize(_width, _height);
    _bloomShader->use();
    _screen->render(_bloomShader);
}

void BloomProcess::freeGPUMemory() {
    _bloomShader->freeGPUMemory();
}

std::shared_ptr<const GLuint> BloomProcess::getRenderTexture() const {
    return nullptr;
}

CstShaderProgram_sptr BloomProcess::createBloomProcessShaderProgram() const {
    CstShaderProgram_sptr sp = ShaderProgram::createShaderProgram("basicFboVs.vs", "bloomFs.fs");
    sp->use();
    sp->bindUniformTexture("frameSceneHDRTexture", 0, _sceneHDRTexture);
    sp->bindUniformTexture("frameBluredTexture", 1, _bluredTexture);
    return sp;
}

