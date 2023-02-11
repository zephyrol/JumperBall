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
    _renderGroupsManager(std::move(renderGroupsManager)),
    _currentRenderGroups(_renderGroupsManager->getRenderGroups()),
    _uniforms(_renderGroupsManager->genUniforms(_shaderProgram))
{
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

void RenderPass::render() const {
    const auto& currentRenderGroups = *_currentRenderGroups;
    for(size_t i = 0; i < currentRenderGroups.size(); ++i){
        // Bind the uniforms related to the group
       _uniforms[i].bind();

       // Render the group
       currentRenderGroups[i].render();
    }
}
