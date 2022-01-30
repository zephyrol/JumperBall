//
// Created by Sébastien Morgenthaler on 28/01/2022.
//

#include "ShapeLabelProcess.h"
#include "componentsGeneration/MeshGenerator.h"

ShapeLabelProcess::ShapeLabelProcess(const CstPage_sptr& page):
_renderPassesShapes(createRenderPassesShapes(page)),
_shapesShader(ShaderProgram::createShaderProgram("labelVs.vs", "labelFs.fs"))
{
}

void ShapeLabelProcess::render() const {
    if(!_renderPassesShapes) {
        return;
    }
    _renderPassesShapes->render(_shapesShader);
}

void ShapeLabelProcess::freeGPUMemory() {
    _shapesShader->freeGPUMemory();
    if(!_renderPassesShapes) {
        return;
    }

    _renderPassesShapes->freeGPUMemory();
}

std::shared_ptr<const GLuint> ShapeLabelProcess::getRenderTexture() const {
    return nullptr;
}

RenderPass_sptr ShapeLabelProcess::createRenderPassesShapes(const CstPage_sptr &page) {

    const auto meshes = MeshGenerator::genShapesLabel(page);
    if (!meshes.empty()) {
        return std::make_shared<RenderPass>(meshes);
    }
    return nullptr;
}

vecCstShaderProgram_sptr ShapeLabelProcess::getShaderPrograms() const {
    return { _shapesShader };
}

