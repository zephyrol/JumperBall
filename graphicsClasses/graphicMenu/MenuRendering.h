/*
 * File: MenuRendering.h
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

class MenuRendering:public Rendering {

public:

MenuRendering(
    const Menu& menu,
    const FontTexturesGenerator::FTContent& ftContent,
    GLsizei width,
    GLsizei height,
    const JBTypes::FileContent& fileContent
    );

void update() override;
void render() const override;
void freeGPUMemory() override;


private:

using PagesProcesses = std::unordered_map <CstPage_sptr, vecRenderProcess_sptr>;
using PagesRenderPassLetters = std::unordered_map <CstPage_sptr, LettersProcess::RenderPassesLetters>;
using PagesRenderPassShapes = std::unordered_map <CstPage_sptr, RenderPass_sptr>;

const Menu& _menu;

const FontTexturesGenerator::GraphicAlphabet _graphicAlphabet;

const PagesRenderPassLetters _pageToRenderPassesLetter;
const PagesRenderPassShapes _pageToRenderPassesShape;
const vecRenderPass_sptr _renderPasses;

const PagesProcesses _pagesProcesses;
const vecRenderProcess_sptr _renderProcesses;
MenuUniformBuffer _menuUniformBuffer;

PagesProcesses createPagesProcesses(
    const JBTypes::FileContent& fileContent
) const;
    vecRenderProcess_sptr createRenderProcesses() const;

    vecCstShaderProgram_sptr getShaderProgramsUsingUniformBuffer() const;

    PagesRenderPassLetters createPageToRenderPassesLetters() const;
    PagesRenderPassShapes createPageToRenderPassesShapes() const;
    vecRenderPass_sptr createRenderPasses() const;
};


#endif // MENURENDERING_H