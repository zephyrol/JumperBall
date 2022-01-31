//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "LettersProcess.h"
#include "componentsGeneration/MeshGenerator.h"

LettersProcess::LettersProcess(
    const CstPage_sptr& page,
    const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet,
    const LettersProcess::RenderPassesLetters& renderPassesLetters
):
    _graphicAlphabet(graphicAlphabet),
    _renderPassesLetters(renderPassesLetters),
    _lettersShader(ShaderProgram::createShaderProgram("fontVs.vs", "fontFs.fs"))
{
}

void LettersProcess::render() const {

    _lettersShader->use();
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
}

std::shared_ptr<const GLuint> LettersProcess::getRenderTexture() const {
    return nullptr;
}

vecCstShaderProgram_sptr LettersProcess::getShaderPrograms() const {
    return { _lettersShader };
}

