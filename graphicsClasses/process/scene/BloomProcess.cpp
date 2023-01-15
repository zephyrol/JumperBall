//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "BloomProcess.h"

#include <utility>

BloomProcess::BloomProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint blurTexture,
    GLint defaultFrameBuffer,
    RenderPass_sptr screen
) :
    _width(width),
    _height(height),
    _screen(std::move(screen)),
    _blurTexture(blurTexture),
    _bloomShader(createBloomProcessShaderProgram(fileContent)),
    _defaultFrameBuffer(defaultFrameBuffer) {
}

void BloomProcess::render() const {
    FrameBuffer::bindDefaultFrameBuffer(_defaultFrameBuffer);
    FrameBuffer::setViewportSize(_width, _height);

    _bloomShader->use();
    ShaderProgram::bindTexture(_blurTexture);
    _screen->render(_bloomShader);
}

void BloomProcess::freeGPUMemory() {
    _bloomShader->freeGPUMemory();
}

std::shared_ptr<const GLuint> BloomProcess::getRenderTexture() const {
    return nullptr;
}

CstShaderProgram_sptr BloomProcess::createBloomProcessShaderProgram(
    const JBTypes::FileContent &fileContent
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "bloomFs.fs",
        {"frameSceneHDRTexture", "frameBluredTexture"}
    );
    shader->use();
    shader->bindUniformTextureIndex("frameSceneHDRTexture", 0);
    shader->bindUniformTextureIndex("frameBluredTexture", 1);
    return shader;
}

vecCstShaderProgram_sptr BloomProcess::getShaderPrograms() const {
    return {_bloomShader};
}
