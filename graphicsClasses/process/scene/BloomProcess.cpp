//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "BloomProcess.h"

BloomProcess::BloomProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint sceneHdrTexture,
    GLuint blurTexture,
    GLint defaultFrameBuffer,
    const CstRenderGroupsManager_sptr &screen
) :
    _width(width),
    _height(height),
    _screenRenderPass(createBloomProcessShaderProgram(fileContent), screen),
    _bloomShader(_screenRenderPass.shaderProgram()),
    _sceneHdrTextureSampler(TextureSampler::createInstance(
        sceneHdrTexture,
        0,
        _bloomShader,
        "frameSceneHDRTexture"
    )),
    _blurTextureSampler(TextureSampler::createInstance(
        blurTexture,
        1,
        _bloomShader,
        "frameBluredTexture")
    ),
    _defaultFrameBuffer(defaultFrameBuffer) {
}

void BloomProcess::render() const {
    FrameBuffer::bindDefaultFrameBuffer(_defaultFrameBuffer);
    FrameBuffer::setViewportSize(_width, _height);

    _bloomShader->use();

    // Perf: hdr texture is already bound in the previous preprocess
    static_cast<void>(_sceneHdrTextureSampler);

    _blurTextureSampler.bind();
    _screenRenderPass.render();
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
        "bloomFs.fs"
    );
    shader->use();
    return shader;
}

vecCstShaderProgram_sptr BloomProcess::getShaderPrograms() const {
    return {_bloomShader};
}
