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
    GLsizei height
    );

void update() override;
void render() const override;
void freeGPUMemory() override;

private:

using PagesLettersProcess = std::unordered_map <CstPage_sptr, std::shared_ptr<LettersProcess> >;
using PagesShapeProcess = std::unordered_map <CstPage_sptr, std::shared_ptr<ShapeLabelProcess> >;

const Menu& _menu;
const FontTexturesGenerator::GraphicAlphabet _graphicAlphabet;

const PagesLettersProcess _pagesLettersProcess;
const PagesShapeProcess  _pagesShapeProcess;
const vecRenderProcess_sptr  _renderProcesses;
MenuUniformBuffer _menuUniformBuffer;

PagesLettersProcess createPagesLettersProcess() const;
PagesShapeProcess createPagesShapeProcess() const;
vecRenderProcess_sptr createRenderProcesses() const;

    vecCstShaderProgram_sptr getShaderProgramsUsingUniformBuffer() const;
};


#endif // MENURENDERING_H