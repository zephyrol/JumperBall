//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "LettersProcess.h"

LettersProcess::LettersProcess(
    const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet,
    const LettersProcess::RenderPassesLetters& renderPassesLetters
):
    _graphicAlphabet(graphicAlphabet),
    _renderPassesLetters(renderPassesLetters),
    _lettersShader(createLettersProcessShaderProgram())
{
}

void LettersProcess::render() const {

    _lettersShader->use();
    ShaderProgram::setActiveTexture(0);
    for(const auto& renderPassLetter: _renderPassesLetters) {
        const auto& renderPass = renderPassLetter.first;
        const auto letter = renderPassLetter.second;

        ShaderProgram::bindTexture(_graphicAlphabet.at(letter).textureID);
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


CstShaderProgram_sptr LettersProcess::createLettersProcessShaderProgram() {
    auto shader = ShaderProgram::createShaderProgram("fontVs.vs", "fontFs.fs");
    shader->use();
    shader->bindUniformTextureIndex("characterTexture", 0);
    return shader;
}
