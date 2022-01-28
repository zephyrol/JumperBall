//
// Created by Sébastien Morgenthaler on 28/01/2022.
//

#include "ShapeLabelProcess.h"

ShapeLabelProcess::ShapeLabelProcess(const RenderPass_sptr &renderPass):
_renderPassesShapes(renderPass),
_shapesShader(ShaderProgram::createShaderProgram("labelVs.vs", "labelFs.fs"))
{
}

void ShapeLabelProcess::render() const {
    _renderPassesShapes->render(_shapesShader);
}

void ShapeLabelProcess::freeGPUMemory() {
    _shapesShader->freeGPUMemory();
    _renderPassesShapes->freeGPUMemory();
}

std::shared_ptr<const GLuint> ShapeLabelProcess::getRenderTexture() const {
    return nullptr;
}

