//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LabelsProcess.h"
#include "componentsGeneration/MeshGenerator.h"

LabelsProcess::LabelsProcess(
    GLsizei width,
    GLsizei height,
    CstPage_sptr page,
    const FontTexturesGenerator &fontTexturesGenerator,
    RenderPass renderPass,
    CstShaderProgram_sptr labelsShader,
    CstMap_sptr map
) :
    Rendering(width, height),
    _page(std::move(page)),
    _fontTexturesGenerator(fontTexturesGenerator),
    _renderPass(std::move(renderPass)),
    _labelsShader(std::move(labelsShader)),
    _uniformFloatNames(_page->getUniformFloatNames()),
    _uniformIntNames(_page->getUniformIntNames()),
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
            label, MeshGenerator::genGeometricShapesFromLabel(*label)
        ));
    }
    RenderPass renderPass(meshes);

    const auto shaderDefine = page->shaderDefines();

    const auto getDefines = [&shaderDefine]() -> std::vector<std::string> {
        if (shaderDefine.empty()) {
            return {};
        }
        return {shaderDefine};
    };
    constexpr auto characterTextureUniformName = "characterTexture";
    std::vector<std::string> uniformsNames = {characterTextureUniformName};
    auto uniformFloatNames = page->getUniformFloatNames();
    uniformsNames.insert(
        uniformsNames.end(),
        std::make_move_iterator(uniformFloatNames.begin()),
        std::make_move_iterator(uniformFloatNames.end())
    );
    auto uniformIntNames = page->getUniformIntNames();
    uniformsNames.insert(
        uniformsNames.end(),
        std::make_move_iterator(uniformIntNames.begin()),
        std::make_move_iterator(uniformIntNames.end())
    );
    auto labelsShader = ShaderProgram::createInstance(
        fileContent,
        page->getVertexShaderName(),
        "labelFs.fs",
        uniformsNames,
        getDefines()
    );

    labelsShader->use();
    labelsShader->bindUniformTextureIndex(characterTextureUniformName, 0);

    return std::unique_ptr<LabelsProcess>(new LabelsProcess(
        width,
        height,
        page,
        fontTexturesGenerator,
        std::move(renderPass),
        std::move(labelsShader),
        std::move(map)
    ));
}

void LabelsProcess::render() const {
    _labelsShader->use();
    ShaderProgram::setActiveTexture(0);
    ShaderProgram::bindTexture(_fontTexturesGenerator.getLettersTexture());
    _renderPass.render(_labelsShader);
}

void LabelsProcess::freeGPUMemory() {
    _labelsShader->freeGPUMemory();
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

    const auto uniformIntValues = _page->getUniformIntValues(_map);
    for (size_t i = 0; i < _uniformIntNames.size(); ++i) {
        _labelsShader->bindUniform(_uniformIntNames[i], uniformIntValues[i]);
    }

    const auto uniformFloatValues = _page->getUniformFloatValues(_map);
    for (size_t i = 0; i < _uniformFloatNames.size(); ++i) {
        _labelsShader->bindUniform(_uniformFloatNames[i], uniformFloatValues[i]);
    }

    _renderPass.update();
}
