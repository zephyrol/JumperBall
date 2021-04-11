/*
 * File: Rendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 avril 2021, 11h35
 */
#include "Rendering.h"

Rendering::Rendering(
    const vecState_sptr& externalStates,
    Rendering::ExternalUniformBlockVariables&& externalUniformBlocks,
    Rendering::ExternalUniformVariables <glm::mat4>&& externalUniformMatrices,
    const vecRenderPass_sptr& renderPasses,
    const vecRenderProcess_sptr& renderingPipeline
    ):
    _externalStates(externalStates),
    _externalUniformBlocks(std::move(externalUniformBlocks)),
    _externalUniformMatrices(std::move(externalUniformMatrices)),
    _renderPasses(renderPasses),
    _renderingPipeline(renderingPipeline) {
}

void Rendering::update() {
    for (const auto& externalState : _externalStates) {
        externalState->update();
    }

    for (auto& externalUniformBlock : _externalUniformBlocks) {
        const RenderPass::UniformBlockVariables_uptr& uniformBlockVariables = externalUniformBlock.first;
        const Rendering::UniformBlockUpdatingFct& updatingFct = externalUniformBlock.second;

        updatingFct(uniformBlockVariables);
    }

    for (auto& externalUniformMatrix : _externalUniformMatrices) {
        const Mesh::UniformVariables_uptr <glm::mat4>& uniformMatrices = externalUniformMatrix.first;
        const Rendering::UniformVariableUpdatingFct <glm::mat4>& updatingFct = externalUniformMatrix.second;

        updatingFct(uniformMatrices);
    }

    for (const auto& renderPass : _renderPasses) {
        renderPass->update();
    }

    for (const auto& renderProcess : _renderingPipeline) {
        renderProcess->updateUniforms();
    }
}

void Rendering::render() const {
    for (const auto& renderProcess : _renderingPipeline) {
        renderProcess->render();
    }
}

CstState_sptr Rendering::getExternalState (size_t number) const {
    return _externalStates.at(number);
}

const glm::mat4& Rendering::getUniformMatrix (size_t number) const {
    const Mesh::UniformVariables_uptr <glm::mat4>&
    return _externalUniformMatrices.at(number).;
}

const RenderPass_sptr& Rendering::getRenderPass (size_t number) const {
    return _renderPasses.at(number);
}
