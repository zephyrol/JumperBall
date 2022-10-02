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
    _ftContent(ftContent),
    _fileContent(fileContent),
    _labelsProcess(_fileContent, _ftContent, width, height, page)
{
}

void PageRendering::update() {
    _labelsProcess.update();
}

void PageRendering::render() const {
    _labelsProcess.render();
}

void PageRendering::freeGPUMemory() {
    _labelsProcess.freeGPUMemory();
}
