/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

RenderPass::RenderPass(
    CstShaderProgram_sptr shaderProgram,
    CstRenderGroup_sptr renderGroup
) :
    _shaderProgram(std::move(shaderProgram)),
    _renderGroup(std::move(renderGroup)),
    _uniforms(_renderGroup->genUniforms(_shaderProgram))
{
}

void RenderPass::update() {
    _uniforms.update();
}

void RenderPass::render() const {
    _uniforms.bind();
    _renderGroup->render();
}
