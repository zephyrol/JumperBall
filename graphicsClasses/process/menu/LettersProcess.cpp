//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "LettersProcess.h"
#include "componentsGeneration/MeshGenerator.h"

LettersProcess::LettersProcess(
        const CstPage_sptr& page,
        const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet
):
        _renderPassesLetters(createRenderPassesLetters(page)),
        _graphicAlphabet(graphicAlphabet),
        _lettersShader(ShaderProgram::createShaderProgram("fontVs.vs", "fontFs.fs"))
{
}

void LettersProcess::render() const {

    for(const auto& renderPassLetter: _renderPassesLetters) {
        const auto& renderPass = renderPassLetter.first;
        const auto letter = renderPassLetter.second;

        constexpr int characterTextureNumber = 0;
        _lettersShader->bindUniformTexture(
            "characterTexture",
            characterTextureNumber,
            _graphicAlphabet.at(letter).textureID
        );
       renderPass->render(_lettersShader);
    }
}

void LettersProcess::freeGPUMemory() {
    _lettersShader->freeGPUMemory();
    for(const auto& renderPassLetter: _renderPassesLetters) {
        const auto& renderPass = renderPassLetter.first;
        renderPass->freeGPUMemory();
    }
}

std::shared_ptr<const GLuint> LettersProcess::getRenderTexture() const {
    return nullptr;
}

LettersProcess::RenderPassesLetters LettersProcess::createRenderPassesLetters(const CstPage_sptr &page) {

    LettersProcess::RenderPassesLetters renderPassesLetters;
    const auto lettersMeshes =  MeshGenerator::genLettersLabel(page, _graphicAlphabet);

    for (const auto& letterMeshes : lettersMeshes) {
        const unsigned char letter = letterMeshes.first;
        const vecMesh_sptr meshes = letterMeshes.second;
        if (!meshes.empty()) {
            renderPassesLetters[std::make_shared <RenderPass>(meshes)] = letter;
        }
    }

    return renderPassesLetters;
}

