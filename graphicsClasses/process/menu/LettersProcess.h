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
        const JBTypes::FileContent& fileContent,
        const FontTexturesGenerator::FTContent &ftContent,
        GLsizei width,
        GLsizei height,
        const CstPage_sptr &page
    );

    void render() const override;
    void freeGPUMemory() override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    FontTexturesGenerator _fontTexturesGenerator;
    //const RenderPassesLetters _renderPassesLetters;
    RenderPass _renderPass;
    const CstShaderProgram_sptr _lettersShader;

    static CstShaderProgram_sptr createLettersProcessShaderProgram(
        const JBTypes::FileContent& fileContent
    );
};


#endif //JUMPERBALLAPPLICATION_LETTERSPROCESS_H
