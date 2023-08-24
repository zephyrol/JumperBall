//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "ShadowProcess.h"

#include <utility>


ShadowProcess::ShadowProcess(
    DepthFrameBuffer_uptr frameBuffer,
    ShaderProgram_sptr shaderProgram,
    RenderPass_sptr renderPass,
    bool isFirst,
    GLsizei depthTextureSize
) :
    _frameBuffer(std::move(frameBuffer)),
    _shaderProgram(std::move(shaderProgram)),
    _renderPass(std::move(renderPass)),
    _isFirst(isFirst),
    _depthTextureSize(depthTextureSize) {
}

ShadowProcess_sptr ShadowProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    CstRenderGroup_sptr map,
    bool isFirst
) {

    const auto shadowDefines =
        isFirst
        ? std::vector<std::string>{"SHADOW_PASS"}
        : std::vector<std::string>{"SHADOW_PASS_2"};

    auto shaderProgram = ShaderProgram::createInstance(
        fileContent,
        "gameSceneVs.vs",
        "depthFs.fs",
        shadowDefines,
        {{"idCount", map->numberOfDynamicsIds()}}
    );

    auto renderPass = std::make_shared<RenderPass>(shaderProgram, map);

    constexpr GLsizei shadowTextureSize = 1024;
    return std::make_shared<ShadowProcess>(
        DepthFrameBuffer::createInstance(
            shadowTextureSize,
            shadowTextureSize
        ),
        std::move(shaderProgram),
        std::move(renderPass),
        isFirst,
        shadowTextureSize
    );
}

void ShadowProcess::render() const {

    if (_isFirst) {
        FrameBuffer::disableBlending();
        glCullFace(GL_FRONT);
        FrameBuffer::enableDepthTest();
        FrameBuffer::setViewportSize(_depthTextureSize, _depthTextureSize);
    }
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clear();

    // for (const auto &shaderRenderPass: _shadersRenderPasses) {
    //     const auto &shader = shaderRenderPass.first;
    //     const auto &renderPass = shaderRenderPass.second;
    //     shader->use();
    //     renderPass->render();
    // }
}

void ShadowProcess::update() {
    //for (const auto &shaderRenderPass: _shadersRenderPasses) {
    //    const auto &renderPass = shaderRenderPass.second;
    //    renderPass->update();
    //}
}

void ShadowProcess::freeGPUMemory() {
    //_frameBuffer->freeGPUMemory();
    //for (auto &shaderRenderPass: _shadersRenderPasses) {
    //    auto &shader = shaderRenderPass.first;
    //    shader->freeGPUMemory();
    //}
}

std::shared_ptr<const GLuint> ShadowProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

vecCstShaderProgram_sptr ShadowProcess::getShaderPrograms() const {
    vecCstShaderProgram_sptr shaderPrograms;
    //for (auto &shaderRenderPass: _shadersRenderPasses) {
    //    auto &shader = shaderRenderPass.first;
    //    shaderPrograms.push_back(shader);
    //}
    return shaderPrograms;
}

GLsizei ShadowProcess::depthTextureSize() const {
    return _depthTextureSize;
}

