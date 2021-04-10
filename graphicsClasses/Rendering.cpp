/*
 * File: Rendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 10 avril 2021, 11h35
 */
#include "Rendering.h"

Rendering::Rendering(
    const vecState_sptr& externalStates,
    ExternalUniformBlockVariables&& externalUniformBlocks,
    ExternalUniformVariables <glm::mat4>&& externalUniformMatrices,
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
        const auto& uniformBlockVariables = externalUniformBlock.first;
        externalUniformBlock.second(uniformBlockVariables);

        for (auto& uniformBlock : *uniformBlockVariables) {
            uniformBlock.second->update();
        }
    }

    for (auto& externalUniformMatrix : _externalUniformMatrices) {
        externalUniformMatrix.second(externalUniformMatrix.first);
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
