/*
 * File: MenuRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 21 avril 2020, 14:50
 */
#include "MenuRendering.h"
#include "process/menu/LettersProcess.h"
#include "process/menu/ShapeLabelProcess.h"

MenuRendering::MenuRendering(
    const Menu& menu,
    const FontTexturesGenerator::FTContent& ftContent,
    GLsizei width,
    GLsizei height
    ):
    Rendering(width, height),
    _menu(menu),
    _graphicAlphabet(FontTexturesGenerator::genGraphicAlphabet(_menu, height, ftContent)),
    _pagesRenderProcess(createPagesRenderProcess()) {
}

void MenuRendering::update() {

    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }
    // TODO: update renderPasses
    // TODO: update Y position

    //letterRenderPass->upsertUniform(shaderProgramID, "positionY", page->localPosY());
}

void MenuRendering::render() const {
    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }
    for (const auto &renderProcess: _pagesRenderProcess.at(currentPage)) {
        renderProcess->render();
    }
}

void MenuRendering::freeGPUMemory() {
    FontTexturesGenerator::freeGraphicAlphabetGPUMemory(_graphicAlphabet);
}


MenuRendering::PagesRenderProcesses MenuRendering::createPagesRenderProcess() const {
    MenuRendering::PagesRenderProcesses pagesRenderProcess;
    for (const auto& page : _menu.pages()) {
        pagesRenderProcess[page] = {
            std::make_shared<ShapeLabelProcess>(page),
            std::make_shared<LettersProcess>(page, _graphicAlphabet),
        };
    }
    return pagesRenderProcess;
}

