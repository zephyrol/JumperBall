//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LABELSPROCESS_H
#define JUMPERBALLAPPLICATION_LABELSPROCESS_H


#include "process/RenderProcess.h"
#include "gameMenu/pages/Page.h"
#include "graphicMenu/FontTexturesGenerator.h"

class LabelsProcess: public RenderProcess {

public:

    LabelsProcess(
        const JBTypes::FileContent& fileContent,
        const FontTexturesGenerator::FTContent &ftContent,
        GLsizei width,
        GLsizei height,
        const CstPage_sptr &page
    );

    void render() const override;
    void update(float pagePositionY, float levelProgression);
    void freeGPUMemory() override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    FontTexturesGenerator _fontTexturesGenerator;
    RenderPass _renderPass;
    const CstShaderProgram_sptr _labelsShader;

    static CstShaderProgram_sptr createLettersProcessShaderProgram(
        const JBTypes::FileContent& fileContent
    );
};


#endif //JUMPERBALLAPPLICATION_LABELSPROCESS_H
