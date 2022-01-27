/*
 * File: RenderProcess.cpp
 * Author: Morgenthaler S
 *
 * Created on 25 mars 2021, 21:48
 */
#include "RenderProcess.h"


/*RenderProcess::RenderProcess(
    const vecRenderPass_sptr& renderPasses,
    PassShaderMap&& shaderPrograms,
    PassUniformUpdateMap&& externalUniformsTreating,
    FrameBuffer_uptr&& frameBuffer,
    bool usingDefaultFrameBufferAutoClean
):
    _renderPasses(renderPasses),
    _shaderPrograms(std::move(shaderPrograms)),
    _externalUniformsTreating(std::move(externalUniformsTreating)),
    _frameBuffer(std::move(frameBuffer)),
    _defaultFrameBufferWidth(nullptr),
    _defaultFrameBufferHeight(nullptr),
    _usingDefaultFrameBufferAutoClean(usingDefaultFrameBufferAutoClean) {
}

RenderProcess::RenderProcess(
    const vecRenderPass_sptr &renderPasses,
    RenderProcess::PassShaderMap &&shaderPrograms,
    RenderProcess::PassUniformUpdateMap &&externalUniformsTreating,
    GLsizei defaultFrameBufferWidth,
    GLsizei defaultFrameBufferHeight,
    bool usingDefaultFrameBufferAutoClean
):
    _renderPasses(renderPasses),
    _shaderPrograms(std::move(shaderPrograms)),
    _externalUniformsTreating(std::move(externalUniformsTreating)),
    _frameBuffer(nullptr),
    _defaultFrameBufferWidth(new GLsizei(defaultFrameBufferWidth)),
    _defaultFrameBufferHeight(new GLsizei(defaultFrameBufferHeight)),
    _usingDefaultFrameBufferAutoClean(usingDefaultFrameBufferAutoClean) {

}*/

/*void RenderProcess::updateUniforms() {
    for (const auto& renderPass : _renderPasses) {
        _externalUniformsTreating.at(renderPass)(renderPass, _shaderPrograms.at(renderPass)->getHandle());
    }
}

void RenderProcess::render() const {

    if (_frameBuffer) {
        _frameBuffer->bindFrameBuffer();
    } else {
        FrameBuffer::bindDefaultFrameBuffer(*_defaultFrameBufferWidth, *_defaultFrameBufferHeight);
        if (_usingDefaultFrameBufferAutoClean) {
            FrameBuffer::cleanDefaultFrameBuffer();
        }
    }
    for (const auto& renderPass : _renderPasses) {
        const CstShaderProgram_sptr& shaderProgram = _shaderPrograms.at(renderPass);
        shaderProgram->use();
        renderPass->render(shaderProgram);
    }
}*/

/*void RenderProcess::freeGPUMemory() {
    if (_frameBuffer) {
        _frameBuffer->freeGPUMemory();
    }
    for (const auto& passShaderProgram : _shaderPrograms) {
        const auto& shaderProgram = passShaderProgram.second;
        shaderProgram->freeGPUMemory();
    }
    for (const auto& renderPass: _renderPasses){
        renderPass->freeGPUMemory();
    }
}

GLuint RenderProcess::getFrameBufferTexture() const {
    return _frameBuffer->getRenderTexture();
}*/


