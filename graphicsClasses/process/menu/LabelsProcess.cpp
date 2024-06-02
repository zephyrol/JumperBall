//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LabelsProcess.h"

#include <utility>
#include "componentsGeneration/RenderGroupGenerator.h"
#include "componentsGeneration/LabelGroupGenerator.h"
#include "frameBuffer/FrameBuffer.h"

LabelsProcess::LabelsProcess(
    GLsizei width,
    GLsizei height,
    CstPage_sptr page,
    FontTexturesGenerator_uptr fontTexturesGenerator,
    RenderGroup_sptr renderGroup,
    ShaderProgram_sptr labelsShader
) :
    Rendering(width, height),
    _page(std::move(page)),
    _fontTexturesGenerator(std::move(fontTexturesGenerator)),
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
    auto fontTexturesGenerator = FontTexturesGenerator::createInstance(width, height, page, ftContent);

    auto labels = page->labels();
    auto messageLabels = fontTexturesGenerator->getTextLabels();
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

    constexpr GLint characterTextureNumber = 5;
    labelsShader->setTextureIndex("characterTexture", characterTextureNumber);
    TextureSampler::setActiveTexture(characterTextureNumber);
    fontTexturesGenerator->getLettersTexture()->bind();

    return std::unique_ptr<LabelsProcess>(
        new LabelsProcess(
            width,
            height,
            page,
            std::move(fontTexturesGenerator),
            std::move(renderGroup),
            std::move(labelsShader)
        )
    );
}

void LabelsProcess::render() const {
    FrameBuffer::enableBlending();
    _labelsShader->use();
    _renderGroupUniform.bind();
    _renderGroup->bind();
    _renderGroup->render();
}

vecCstShaderProgram_sptr LabelsProcess::getShaderPrograms() const {
    return {_labelsShader};
}

void LabelsProcess::update() {
    _renderGroupUniform.update();
}
