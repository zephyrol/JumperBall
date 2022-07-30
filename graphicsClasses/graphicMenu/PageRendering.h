/*
 * File: PageRendering.h
 * Author: Morgenthaler S
 *
 * Created on 21 avril 2020, 14:50
 */
#ifndef MENURENDERING_H
#define MENURENDERING_H

#include <gameMenu/Menu.h>
#include "Rendering.h"
#include "FontTexturesGenerator.h"
#include "componentsGeneration/MeshGenerator.h"
#include "process/menu/LettersProcess.h"
#include "process/menu/ShapeLabelProcess.h"
#include "process/menu/MenuUniformBuffer.h"

class PageRendering : public Rendering {

public:

    PageRendering(
        const CstPage_sptr &page,
        const FontTexturesGenerator::FTContent &ftContent,
        GLsizei width,
        GLsizei height,
        const JBTypes::FileContent &fileContent
    );

    void update() override;

    void render() const override;

    void freeGPUMemory() override;

private:

    const CstPage_sptr& _page;
    const FontTexturesGenerator::FTContent &_ftContent;
    const JBTypes::FileContent &_fileContent;

    const CstPage_sptr _currentPage;
    LettersProcess _lettersProcess;

    MenuUniformBuffer _pageUniformBuffer;

    vecRenderProcess_sptr createRenderProcesses() const;

    vecCstShaderProgram_sptr getShaderProgramsUsingUniformBuffer() const;
};


#endif // MENURENDERING_H
