//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "BloomProcess.h"

BloomProcess::BloomProcess(
    const JBTypes::FileContent& fileContent,
    GLsizei width,
    GLsizei height,
    GLuint bluredTexture,
    GLint defaultFrameBuffer,
    const RenderPass_sptr& screen
):
        _width(width),
        _height(height),
        _screen(screen),
        _bluredTexture(bluredTexture),
        _bloomShader(createBloomProcessShaderProgram(fileContent)),
        _defaultFrameBuffer(defaultFrameBuffer)
{
}

void BloomProcess::render() const {
    FrameBuffer::bindDefaultFrameBuffer(_defaultFrameBuffer);
    FrameBuffer::setViewportSize(_width, _height);

    _bloomShader->use();
    ShaderProgram::bindTexture(_bluredTexture);
    _screen->render(_bloomShader);
    //FrameBuffer::cleanDefaultFrameBuffer();
}

void BloomProcess::freeGPUMemory() {
    _bloomShader->freeGPUMemory();
}

std::shared_ptr<const GLuint> BloomProcess::getRenderTexture() const {
    return nullptr;
}

CstShaderProgram_sptr BloomProcess::createBloomProcessShaderProgram(
    const JBTypes::FileContent& fileContent
) {
    auto shader = ShaderProgram::createShaderProgram(fileContent,"basicFboVs.vs", "bloomFs.fs");
    shader->use();
    shader->bindUniformTextureIndex("frameSceneHDRTexture", 0);
    shader->bindUniformTextureIndex("frameBluredTexture", 1);
    return shader;
}

vecCstShaderProgram_sptr BloomProcess::getShaderPrograms() const {
    return { _bloomShader };
}

