//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "LettersProcess.h"
#include "componentsGeneration/MeshGenerator.h"

LettersProcess::LettersProcess(
    const JBTypes::FileContent& fileContent,
    const FontTexturesGenerator::FTContent &ftContent,
    GLsizei width,
    GLsizei height,
    const CstPage_sptr &page
):
    _fontTexturesGenerator(width, height, page, ftContent),
    _renderPass([this, &page](){
        vecMesh_sptr meshes;
        const auto& messageLabels = _fontTexturesGenerator.getMessageLabels();
        for(const auto& messageLabel: messageLabels) {
            Mesh_sptr mesh = std::make_shared<Mesh>(
                page, MeshGenerator::genGeometricShapesFromLabel(*messageLabel)
            );
            meshes.push_back(mesh);
        }
        return meshes;
    }()),
    _lettersShader(createLettersProcessShaderProgram(fileContent))
{
}

void LettersProcess::render() const {

    _lettersShader->use();
    ShaderProgram::setActiveTexture(0);
    /*for(const auto& renderPassLetter: _renderPassesLetters) {
        const auto& renderPass = renderPassLetter.first;
        const auto letter = renderPassLetter.second;

        ShaderProgram::bindTexture(_graphicAlphabet.at(letter).textureID);
        renderPass->render(_lettersShader);
    }*/
    _renderPass.render(_lettersShader);
}

void LettersProcess::freeGPUMemory() {
    _lettersShader->freeGPUMemory();
    _fontTexturesGenerator.freeGPUMemory();
}

std::shared_ptr<const GLuint> LettersProcess::getRenderTexture() const {
    return nullptr;
}

vecCstShaderProgram_sptr LettersProcess::getShaderPrograms() const {
    return { _lettersShader };
}


CstShaderProgram_sptr LettersProcess::createLettersProcessShaderProgram(
    const JBTypes::FileContent& fileContent
) {
    auto shader = ShaderProgram::createShaderProgram(
        fileContent,
        "fontVs.vs",
        "fontFs.fs"
        );
    shader->use();
    shader->bindUniformTextureIndex("characterTexture", 0);
    return shader;
}
