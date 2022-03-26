/*
 * File: Viewer.cpp
 * Author: Morgenthaler S
 */

#include "Viewer.h"

Viewer::Viewer(
    unsigned int resolutionX,
    unsigned int resolutionY,
    GLint defaultFrameBuffer,
    const Scene& scene,
    const Menu& menu,
    const FontTexturesGenerator::FTContent &ftContent,
    const JBTypes::FileContent& fileContent
):
        _resolutionX(resolutionX),
        _resolutionY(resolutionY),
        _defaultFrameBuffer(defaultFrameBuffer),
        _sceneRendering(
            scene,
            _resolutionX,
            _resolutionY,
            defaultFrameBuffer,
            fileContent
        ),
        _menuRendering(menu, ftContent, _resolutionX, _resolutionY, fileContent)
{
}

void Viewer::update() {
    _sceneRendering.update();
    _menuRendering.update();
}

void Viewer::render() const {
    _sceneRendering.render();
    _menuRendering.render();
    //FrameBuffer::cleanDefaultFrameBuffer();
}

void Viewer::freeGPUMemory() {
    _sceneRendering.freeGPUMemory();
    _menuRendering.freeGPUMemory();
}
