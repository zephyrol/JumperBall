//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LabelsProcess.h"
#include "componentsGeneration/MeshGenerator.h"

LabelsProcess::LabelsProcess(
    const JBTypes::FileContent &fileContent,
    const FontTexturesGenerator::FTContent &ftContent,
    GLsizei width,
    GLsizei height,
    const CstPage_sptr &page,
    CstMap_sptr map
) : Rendering(width, height),
    _page(page),
    _uniformNames(_page->getUniformNames()),
    _fontTexturesGenerator(FontTexturesGenerator::createInstance(width, height, page, ftContent)),
    _renderPass([this, &page]() {
        vecMesh_sptr meshes;
        auto labels = page->labels();
        auto messageLabels = _fontTexturesGenerator.getTextLabels();
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
        return meshes;
    }()),
    _labelsShader(createLettersProcessShaderProgram(fileContent, _page)),
    _map(std::move(map)){
}

void LabelsProcess::render() const {

    _labelsShader->use();
    ShaderProgram::setActiveTexture(0);
    _labelsShader->bindUniformTextureIndex("characterTexture", 0);
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


CstShaderProgram_sptr LabelsProcess::createLettersProcessShaderProgram(
    const JBTypes::FileContent &fileContent,
    const CstPage_sptr &page
) {
    const auto shaderDefine = page->shaderDefines();

    const auto getDefines = [&shaderDefine]() -> std::vector<std::string> {
        if (shaderDefine.empty()) {
            return {};
        }
        return {shaderDefine};
    };
    auto shader = ShaderProgram::createShaderProgram(
        fileContent,
        page->getVertexShaderName(),
        "labelFs.fs",
        getDefines()
        //"labelVs.vs",
        //"labelFs.fs"
    );
    return shader;
}

void LabelsProcess::update() {
    _labelsShader->use();

    const auto uniformValues = _page->getUniformValues(_map);
    for (size_t i = 0; i < _uniformNames.size(); ++i) {
        _labelsShader->bindUniform(_uniformNames[i], uniformValues[i]);
    }

    _renderPass.update();
}

void LabelsProcess::setMap(CstMap_sptr map) {
    _map = std::move(map);
}
