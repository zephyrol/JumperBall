/*
 * File: RenderProcess.cpp
 * Author: Morgenthaler S
 *
 * Created on 25 mars 2021, 21:48
 */
#include "RenderProcess.h"


RenderProcess::RenderProcess(
    const vecRenderPass_sptr& renderPasses,
    std::map <RenderPass_sptr, CstShaderProgram_uptr>&& shaderPrograms,
    std::map <RenderPass_sptr, UniformUpdatingFct>&& externalUniformsTreating,
    FrameBuffer_uptr&& frameBuffer):
    _renderPasses(renderPasses),
    _shaderPrograms(std::move(shaderPrograms)),
    _externalUniformsTreating(std::move(externalUniformsTreating)),
    _frameBuffer(std::move(frameBuffer)) {
}

void RenderProcess::updateUniforms() {
    for (const auto& renderPass : _renderPasses) {
        _externalUniformsTreating.at(renderPass)(renderPass);
    }
}

void RenderProcess::render() const {

    if (_frameBuffer) {
        _frameBuffer->bindFrameBuffer(true);
    } else {
        FrameBuffer::bindDefaultFrameBuffer(true);
    }
    for (const auto& renderPass : _renderPasses) {
        const CstShaderProgram_uptr& shaderProgram = _shaderPrograms.at(renderPass);
        shaderProgram->use();
        renderPass->render(shaderProgram);
    }
}
