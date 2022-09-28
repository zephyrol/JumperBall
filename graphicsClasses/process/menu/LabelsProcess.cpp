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
    const CstPage_sptr &page
) :
    _fontTexturesGenerator(FontTexturesGenerator::createInstance(width, height, page, ftContent)),
    _renderPass([this, &page]() {
        vecMesh_sptr meshes;
        auto labels = page->labels();
        auto messageLabels = _fontTexturesGenerator.getMessageLabels();
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
    _labelsShader(createLettersProcessShaderProgram(fileContent)) {
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
    const JBTypes::FileContent &fileContent
) {
    auto shader = ShaderProgram::createShaderProgram(
        fileContent,
        "fontVs.vs",
        "fontFs.fs"
        //"labelVs.vs",
        //"labelFs.fs"
    );
    return shader;
}

void LabelsProcess::update(float pagePositionY, float levelProgression) {
    _labelsShader->use();
    _labelsShader->bindUniform("positionY", pagePositionY);
    _labelsShader->bindUniform("levelProgression", levelProgression);
    _renderPass.update();
}
