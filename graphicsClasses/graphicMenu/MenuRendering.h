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

class MenuRendering:public Rendering {

public:

MenuRendering(
    const Menu& menu,
    const FontTexturesGenerator::FTContent& ftContent,
    GLsizei width,
    GLsizei height
    );

void update() override;
void render() const override;
void freeGPUMemory() override;

private:

using PagesRenderProcesses = std::unordered_map <CstPage_sptr, vecRenderProcess_sptr>;

const Menu& _menu;
const FontTexturesGenerator::GraphicAlphabet _graphicAlphabet;

const PagesRenderProcesses _pagesRenderProcess;
PagesRenderProcesses createPagesRenderProcess() const;
};


#endif // MENURENDERING_H