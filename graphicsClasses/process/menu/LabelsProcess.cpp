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
    RenderGroup_sptr renderGroup,
    RenderPass renderPass,
    ShaderProgram_sptr labelsShader
) :
    Rendering(width, height),
    _page(std::move(page)),
    _fontTexturesGenerator(fontTexturesGenerator),
    _renderGroup(std::move(renderGroup)),
    _renderPass(std::move(renderPass)),
    _labelsShader(std::move(labelsShader)) {
}

std::unique_ptr<LabelsProcess> LabelsProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    const FontTexturesGenerator::FTContent &ftContent,
    GLsizei width,
    GLsizei height,
    const CstPage_sptr &page
) {

    auto fontTexturesGenerator(FontTexturesGenerator::createInstance(width, height, page, ftContent));

    vecCstMesh_sptr meshes;
    auto labels = page->labels();
    auto messageLabels = fontTexturesGenerator.getTextLabels();
    labels.insert(
        labels.end(),
        std::make_move_iterator(messageLabels.begin()),
        std::make_move_iterator(messageLabels.end())
    );
    for (const auto &label: labels) {
        meshes.push_back(std::make_shared<Mesh>(
            std::make_shared<RenderableLabel>(label, page), MeshGenerator::genGeometricShapesFromLabel(*label), 0
        ));
    }

    std::vector<std::pair<std::string, GLfloat>> glFloatConsts{};
    for (const auto &item: page->getVertexShaderConstants()) {
        glFloatConsts.emplace_back(item.first, Utility::convertToOpenGLFormat(item.second));
    }

    auto renderGroup = RenderGroup::createInstance(
        std::unique_ptr<MeshDynamicGroup>(new MeshDynamicGroup({meshes}))
    );

    auto labelsShader = ShaderProgram::createInstance(
        fileContent,
        page->getVertexShaderName(),
        "labelFs.fs",
        page->shaderDefines(),
        {{"idCount", renderGroup->numberOfDynamicsIds()}},
        glFloatConsts
    );
    labelsShader->use();

    RenderPass renderPass(labelsShader, renderGroup);

    labelsShader->setTextureIndex("characterTexture", 0);

    return std::unique_ptr<LabelsProcess>(new LabelsProcess(
        width,
        height,
        page,
        fontTexturesGenerator,
        renderGroup,
        std::move(renderPass),
        std::move(labelsShader)
    ));
}

void LabelsProcess::render() const {
    FrameBuffer::enableBlending();
    _labelsShader->use();
    TextureSampler::setActiveTexture(0);
    TextureSampler::bind(_fontTexturesGenerator.getLettersTexture());
    _renderPass.render();
}

void LabelsProcess::freeGPUMemory() {
    _labelsShader->freeGPUMemory();
    _renderGroup->freeGPUMemory();
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
    _renderPass.update();
}
