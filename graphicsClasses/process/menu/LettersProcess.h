//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LETTERSPROCESS_H
#define JUMPERBALLAPPLICATION_LETTERSPROCESS_H


#include "process/RenderProcess.h"
#include "gameMenu/Page.h"
#include "graphicMenu/FontTexturesGenerator.h"

class LettersProcess: public RenderProcess {

public:

    using RenderPassesLetters = std::unordered_map <RenderPass_sptr, unsigned char>;

    LettersProcess(
        const CstPage_sptr& page,
        const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const RenderPassesLetters _renderPassesLetters;
    const FontTexturesGenerator::GraphicAlphabet& _graphicAlphabet;
    const CstShaderProgram_sptr _lettersShader;

    RenderPassesLetters createRenderPassesLetters(const CstPage_sptr& page);
};


#endif //JUMPERBALLAPPLICATION_LETTERSPROCESS_H
