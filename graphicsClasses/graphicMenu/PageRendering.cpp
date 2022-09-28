/*
 * File: PageRendering.cpp
 * Author: S.Morgenthaler
 *
 * Created on 21 avril 2020, 14:50
 */
#include "PageRendering.h"

PageRendering::PageRendering(
    const CstPage_sptr &page,
    const FontTexturesGenerator::FTContent &ftContent,
    GLsizei width,
    GLsizei height,
    const JBTypes::FileContent &fileContent
) :
    Rendering(width, height),
    _page(page),
    _ftContent(ftContent),
    _fileContent(fileContent),
    _lettersProcess(_fileContent, _ftContent, width, height, page)
{
}

void PageRendering::update() {
    // _pageUniformBuffer.update(_page->localPosY());
    _lettersProcess.update(_page->localPosY());
}

void PageRendering::render() const {
    // _pageUniformBuffer.bindBufferRange();
    _lettersProcess.render();
}

void PageRendering::freeGPUMemory() {
    _lettersProcess.freeGPUMemory();
    // _pageUniformBuffer.freeGPUMemory();
}
