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
    _pageToRenderPassesLetter(createPageToRenderPassesLetters()),
    _pageToRenderPassesShape(createPageToRenderPassesShapes()),
    _pagesProcesses(createPagesProcesses()),
    _renderProcesses(createRenderProcesses()),
    _menuUniformBuffer(getShaderProgramsUsingUniformBuffer()){

    // TODO: Move thoses instructions
    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);
}

void MenuRendering::update() {

    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }
    for (const auto &renderPass: _renderPasses) {
        renderPass->update();
    }
    _menuUniformBuffer.update(currentPage->localPosY());
}

void MenuRendering::render() const {
    const auto& currentPage = _menu.currentPage();
    if (!currentPage) {
        return;
    }
    /*FrameBuffer::bindDefaultFrameBuffer();
    FrameBuffer::disableDepthTest();
    FrameBuffer::setViewportSize(_width, _height);
    FrameBuffer::cleanDefaultFrameBuffer();*/
    for (const auto& process: _pagesProcesses.at(currentPage)) {
        process->render();
    }
}

void MenuRendering::freeGPUMemory() {

    for (const auto &renderProcess: _renderProcesses) {
        renderProcess->freeGPUMemory();
    }

    for(const auto& renderPass: _renderPasses) {
        renderPass->freeGPUMemory();
    }
    _menuUniformBuffer.freeGPUMemory();
    FontTexturesGenerator::freeGraphicAlphabetGPUMemory(_graphicAlphabet);
}


vecRenderProcess_sptr MenuRendering::createRenderProcesses() const {
    vecRenderProcess_sptr renderProcesses;
    for (const auto &pageProcesses: _pagesProcesses) {
        const auto& processes = pageProcesses.second;
        renderProcesses.insert(renderProcesses.end(), processes.begin(), processes.end());
    }
    return renderProcesses;
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

MenuRendering::PagesProcesses MenuRendering::createPagesProcesses() const {
    MenuRendering::PagesProcesses pagesProcesses;
    for (const auto& page : _menu.pages()) {
        pagesProcesses[page] = {};
        const auto& renderPassLetter = _pageToRenderPassesLetter.find(page);
        if(renderPassLetter != _pageToRenderPassesLetter.end()) {
            pagesProcesses[page].push_back(
                std::make_shared<LettersProcess>(page, _graphicAlphabet, renderPassLetter->second)
            );
        }
        const auto& renderPassShape = _pageToRenderPassesShape.find(page);
        if(renderPassShape != _pageToRenderPassesShape.end()) {
            pagesProcesses[page].push_back(
                std::make_shared<ShapeLabelProcess>(page, renderPassShape->second)
            );
        }
}
return pagesProcesses;
}

MenuRendering::PagesRenderPassLetters MenuRendering::createPageToRenderPassesLetters() const {
    MenuRendering::PagesRenderPassLetters pagesRenderPassLetters;
    for (const auto &page: _menu.pages()) {
        LettersProcess::RenderPassesLetters renderPassesLetters;
        const auto lettersMeshes =  MeshGenerator::genLettersLabel(page, _graphicAlphabet);

        for (const auto& letterMeshes : lettersMeshes) {
            const unsigned char letter = letterMeshes.first;
            const vecMesh_sptr meshes = letterMeshes.second;
            if (!meshes.empty()) {
                renderPassesLetters[std::make_shared <RenderPass>(meshes)] = letter;
            }
        }
        pagesRenderPassLetters[page] = renderPassesLetters;
    }
    return pagesRenderPassLetters;
}

MenuRendering::PagesRenderPassShapes MenuRendering::createPageToRenderPassesShapes() const {
    MenuRendering::PagesRenderPassShapes  pagesRenderPassShapes;
    for (const auto &page: _menu.pages()) {
        const auto meshes = MeshGenerator::genShapesLabel(page);
        if (!meshes.empty()) {
            pagesRenderPassShapes[page] = std::make_shared<RenderPass>(meshes);
        }
    }
    return pagesRenderPassShapes;
}

vecRenderPass_sptr MenuRendering::createRenderPasses() const {
    vecRenderPass_sptr renderPasses;
    for (const auto &pageRenderPass: _pageToRenderPassesShape) {
        renderPasses.push_back(pageRenderPass.second);
    }
    for (const auto &pageRenderPassesLetters: _pageToRenderPassesLetter) {
        const auto& renderPassesLetters = pageRenderPassesLetters.second;
        for (const auto &renderPassLetter: renderPassesLetters) {
            renderPasses.push_back(renderPassLetter.first);
        }
    }
    return renderPasses;
}

