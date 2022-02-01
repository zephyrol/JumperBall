//
// Created by Sébastien Morgenthaler on 28/01/2022.
//

#include "ShapeLabelProcess.h"
#include "componentsGeneration/MeshGenerator.h"

ShapeLabelProcess::ShapeLabelProcess(const RenderPass_sptr& renderPassShapes):
    _renderPassShapes(renderPassShapes),
    _shapesShader(ShaderProgram::createShaderProgram("labelVs.vs", "labelFs.fs"))
{
}

void ShapeLabelProcess::render() const {
    _shapesShader->use();
    _renderPassShapes->render(_shapesShader);
}

void ShapeLabelProcess::freeGPUMemory() {
    _shapesShader->freeGPUMemory();
    _renderPassShapes->freeGPUMemory();
}

std::shared_ptr<const GLuint> ShapeLabelProcess::getRenderTexture() const {
    return nullptr;
}

vecCstShaderProgram_sptr ShapeLabelProcess::getShaderPrograms() const {
    return { _shapesShader };
}

