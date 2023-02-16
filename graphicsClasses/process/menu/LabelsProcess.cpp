//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LabelsProcess.h"

#include <utility>
#include "componentsGeneration/MeshGenerator.h"
#include "gameMenu/labels/RenderableLabel.h"

LabelsProcess::LabelsProcess(
    GLsizei width,
    GLsizei height,
    CstPage_sptr page,
    const FontTexturesGenerator &fontTexturesGenerator,
    RenderGroupsManager_sptr renderGroupsManager,
    RenderPass renderPass,
    ShaderProgram_sptr labelsShader,
    CstMap_sptr map
) :
    Rendering(width, height),
    _page(std::move(page)),
    _fontTexturesGenerator(fontTexturesGenerator),
    _renderGroupsManager(std::move(renderGroupsManager)),
    _renderPass(std::move(renderPass)),
    _labelsShader(std::move(labelsShader)),
    _map(std::move(map)) {
}

std::unique_ptr<LabelsProcess> LabelsProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    const FontTexturesGenerator::FTContent &ftContent,
    GLsizei width,
    GLsizei height,
    const CstPage_sptr &page,
    CstMap_sptr map
) {

    auto fontTexturesGenerator(FontTexturesGenerator::createInstance(width, height, page, ftContent));

    vecMesh_sptr meshes;
    auto labels = page->labels();
    auto messageLabels = fontTexturesGenerator.getTextLabels();
    labels.insert(
        labels.end(),
        std::make_move_iterator(messageLabels.begin()),
        std::make_move_iterator(messageLabels.end())
    );
    for (const auto &label: labels) {
        meshes.push_back(std::make_shared<Mesh>(
            std::make_shared<RenderableLabel>(label, page), MeshGenerator::genGeometricShapesFromLabel(*label)
        ));
    }

    auto labelsShader = ShaderProgram::createInstance(
        fileContent,
        page->getVertexShaderName(),
        "labelFs.fs",
        page->shaderDefines()
    );
    labelsShader->use();

    auto renderGroupsManager = std::make_shared<RenderGroupsManager>(meshes);
    RenderPass renderPass(labelsShader, renderGroupsManager);

    labelsShader->setTextureIndex("characterTexture", 0);

    return std::unique_ptr<LabelsProcess>(new LabelsProcess(
        width,
        height,
        page,
        fontTexturesGenerator,
        renderGroupsManager,
        std::move(renderPass),
        std::move(labelsShader),
        std::move(map)
    ));
}

void LabelsProcess::render() const {
    _labelsShader->use();
    TextureSampler::setActiveTexture(0);
    TextureSampler::bind(_fontTexturesGenerator.getLettersTexture());
    _renderPass.render();
}

void LabelsProcess::freeGPUMemory() {
    _labelsShader->freeGPUMemory();
    _renderGroupsManager->freeGPUMemory();
    _fontTexturesGenerator.freeGPUMemory();
}

std::shared_ptr<const GLuint> LabelsProcess::getRenderTexture() const {
    return nullptr;
}

vecCstShaderProgram_sptr LabelsProcess::getShaderPrograms() const {
    return {_labelsShader};
}

void LabelsProcess::update() {
    _labelsShader->use();
    _renderGroupsManager->update();
    _renderPass.update();
}
