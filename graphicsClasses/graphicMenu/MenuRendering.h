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

using RenderPassesLetters = std::map <RenderPass_sptr, unsigned char>;

using PagesRenderPassesLetters = std::map <CstPage_sptr, RenderPassesLetters>;
using PagesRenderPasses = std::map <CstPage_sptr, vecRenderPass_sptr>;
using PagesRenderProcess = std::map <CstPage_sptr, RenderProcess_sptr>;

const Menu& _menu;
const FontTexturesGenerator::GraphicAlphabet _graphicAlphabet;
const PagesRenderPassesLetters _pagesRenderPassesLetters;
const PagesRenderPasses _pagesRenderPassesOthers;

const PagesRenderPasses _pagesRenderPasses;

const PagesRenderProcess _pagesRenderProcess;


Rendering::ExternalUniformVariables <GLuint> _externalUniformTextures;

PagesRenderPassesLetters createPagesRenderPassesLetters() const;
PagesRenderPasses createPagesRenderPassesOthers() const;
PagesRenderPasses createPagesRenderPasses() const;
RenderProcess_sptr createRenderProcess(const CstPage_sptr& page) const;
PagesRenderProcess createPagesRenderProcess() const;
Rendering::ExternalUniformVariables <GLuint> createExternalUniformTexturesVariables() const;
// vecRenderPass_sptr createRenderPasses() const;
};


#endif // MENURENDERING_H