//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "BloomProcess.h"

BloomProcess::BloomProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint blurTexture,
    GLint defaultFrameBuffer,
    const CstRenderGroupsManager_sptr &screen
) :
    _width(width),
    _height(height),
    _bloomShader(createBloomProcessShaderProgram(fileContent)),
    _screenRenderPass(_bloomShader, screen),
    _blurTexture(blurTexture),
    _defaultFrameBuffer(defaultFrameBuffer) {
}

void BloomProcess::render() const {
    FrameBuffer::bindDefaultFrameBuffer(_defaultFrameBuffer);
    FrameBuffer::setViewportSize(_width, _height);

    _bloomShader->use();
    TextureSampler::bind(_blurTexture);
    _screenRenderPass.render();
}

std::shared_ptr<const GLuint> BloomProcess::getRenderTexture() const {
    return nullptr;
}

ShaderProgram_sptr BloomProcess::createBloomProcessShaderProgram(
    const JBTypes::FileContent &fileContent
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "basicFboVs.vs",
        "bloomFs.fs"
    );
    shader->use();
    shader->setTextureIndex("frameSceneHDRTexture", 0);
    shader->setTextureIndex("frameBluredTexture", 1);
    return shader;
}

void BloomProcess::freeGPUMemory() {
    _bloomShader->freeGPUMemory();
}

vecCstShaderProgram_sptr BloomProcess::getShaderPrograms() const {
    return {_bloomShader};
}
