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
    _pagesLettersProcess(createPagesLettersProcess()),
    _pagesShapeProcess(createPagesShapeProcess()),
    _renderProcesses(createRenderProcesses()),
    _menuUniformBuffer(getShaderProgramsUsingUniformBuffer()){
}

void MenuRendering::update() {

    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }
    _menuUniformBuffer.update(currentPage->localPosY());
}

void MenuRendering::render() const {
    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }
    const auto& shapeProcess = _pagesShapeProcess.at(currentPage);
    shapeProcess->render();

    const auto& lettersProcess = _pagesShapeProcess.at(currentPage);
    lettersProcess->render();

}

void MenuRendering::freeGPUMemory() {
    FontTexturesGenerator::freeGraphicAlphabetGPUMemory(_graphicAlphabet);
    for (const auto &renderProcess: _renderProcesses) {
        renderProcess->freeGPUMemory();
    }
}


vecRenderProcess_sptr MenuRendering::createRenderProcesses() const {
    vecRenderProcess_sptr renderProcesses;
    for (const auto &pageLetterProcess: _pagesLettersProcess) {
        renderProcesses.push_back(pageLetterProcess.second);
    }
    for (const auto &pageShapeProcess: _pagesShapeProcess) {
        renderProcesses.push_back(pageShapeProcess.second);
    }
    return renderProcesses;
}

MenuRendering::PagesLettersProcess MenuRendering::createPagesLettersProcess() const {
    MenuRendering::PagesLettersProcess pagesLettersProcess;
    for (const auto& page : _menu.pages()) {
        pagesLettersProcess[page] = std::make_shared<LettersProcess>(page, _graphicAlphabet);
    }
    return pagesLettersProcess;
}

MenuRendering::PagesShapeProcess MenuRendering::createPagesShapeProcess() const {
    MenuRendering::PagesShapeProcess pagesShapeProcess;
    for (const auto& page : _menu.pages()) {
        pagesShapeProcess[page] = std::make_shared<ShapeLabelProcess>(page);
    }
    return pagesShapeProcess;
}

vecCstShaderProgram_sptr MenuRendering::getShaderProgramsUsingUniformBuffer() const {
    vecCstShaderProgram_sptr shadersPrograms {};
    for (const auto &renderProcess: _renderProcesses) {
        const auto& processShaderPrograms = renderProcess->getShaderPrograms();
        shadersPrograms.insert(
            shadersPrograms.end(),
            processShaderPrograms.begin(),
            processShaderPrograms.end()
        );
    }
    return shadersPrograms;
}

