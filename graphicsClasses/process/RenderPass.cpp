/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

RenderPass::RenderPass(
    CstShaderProgram_sptr shaderProgram,
    CstRenderGroupsManager_sptr renderGroupsManager
) :
    _shaderProgram(std::move(shaderProgram)),
    _renderGroupsManager(std::move(renderGroupsManager)) {
}

void RenderPass::update() {
    const auto renderGroups = _renderGroupsManager->getRenderGroups();
    if (_currentRenderGroups != renderGroups) {
        _uniforms = _renderGroupsManager->genUniforms(_shaderProgram);
        _currentRenderGroups = renderGroups;
        return;
    }
    for (auto &meshUniforms: _uniforms) {
        meshUniforms.update();
    }
}

void RenderPass::bindUniforms() const {
    for (const auto &meshUniforms: _uniforms) {
        meshUniforms.bind();
    }
}

void RenderPass::render() const {
    bindUniforms();
    _renderGroupsManager->render();
}

const CstShaderProgram_sptr &RenderPass::shaderProgram() const {
    return _shaderProgram;
}
