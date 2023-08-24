//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LabelsProcess.h"

#include <utility>
#include "componentsGeneration/RenderGroupGenerator.h"
#include "gameMenu/labels/RenderableLabel.h"
#include "componentsGeneration/LabelGroupGenerator.h"
#include "frameBuffer/FrameBuffer.h"

LabelsProcess::LabelsProcess(
    GLsizei width,
    GLsizei height,
    CstPage_sptr page,
    const FontTexturesGenerator &fontTexturesGenerator,
    RenderGroup_sptr renderGroup,
    ShaderProgram_sptr labelsShader
) :
    Rendering(width, height),
    _page(std::move(page)),
    _fontTexturesGenerator(fontTexturesGenerator),
    _renderGroup(std::move(renderGroup)),
    _labelsShader(std::move(labelsShader)),
    _renderGroupUniform(_renderGroup->genUniforms(_labelsShader)) {
}

std::unique_ptr<LabelsProcess> LabelsProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    const FontTexturesGenerator::FTContent &ftContent,
    GLsizei width,
    GLsizei height,
    const CstPage_sptr &page
) {

    auto fontTexturesGenerator(FontTexturesGenerator::createInstance(width, height, page, ftContent));

    auto labels = page->labels();
    auto messageLabels = fontTexturesGenerator.getTextLabels();
    labels.insert(
        labels.end(),
        std::make_move_iterator(messageLabels.begin()),
        std::make_move_iterator(messageLabels.end())
    );

    std::vector<std::pair<std::string, GLfloat>> glFloatConsts{};
    for (const auto &item: page->getVertexShaderConstants()) {
        glFloatConsts.emplace_back(item.first, Utility::convertToOpenGLFormat(item.second));
    }

    auto renderGroup = LabelGroupGenerator(std::move(labels), page).genRenderGroup();

    auto labelsShader = ShaderProgram::createInstance(
        fileContent,
        page->getVertexShaderName(),
        "labelFs.fs",
        page->shaderDefines(),
        {{"idCount", renderGroup->numberOfDynamicsIds()}},
        glFloatConsts
    );
    labelsShader->use();
    labelsShader->setTextureIndex("characterTexture", 0);

    return std::unique_ptr<LabelsProcess>(new LabelsProcess(
        width,
        height,
        page,
        fontTexturesGenerator,
        std::move(renderGroup),
        std::move(labelsShader)
    ));
}

void LabelsProcess::render() const {
    FrameBuffer::enableBlending();
    _labelsShader->use();
    TextureSampler::setActiveTexture(0);
    TextureSampler::bind(_fontTexturesGenerator.getLettersTexture());
    _renderGroupUniform.bind();
    _renderGroup->bind();
    _renderGroup->render();
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
    _renderGroupUniform.update();
}
