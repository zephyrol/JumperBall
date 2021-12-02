/*
 * File: Viewer.cpp
 * Author: Morgenthaler S
 */

#include "Viewer.h"

Viewer::Viewer(
    unsigned int resolutionX,
    unsigned int resolutionY,
    const Scene& scene,
    const Menu& menu,
    const FontTexturesGenerator::FTContent &ftContent):
        _resolutionX(resolutionX),
        _resolutionY(resolutionY),
        _sceneRendering(scene, _resolutionX, _resolutionY),
        _menuRendering(menu, ftContent, _resolutionX, _resolutionY)
{
}

void Viewer::update() {
    _sceneRendering.update();
    _menuRendering.update();
}

void Viewer::render() const {
    _sceneRendering.render();
    _menuRendering.render();
}

void Viewer::freeGPUMemory() {
    _sceneRendering.freeGPUMemory();
    _menuRendering.freeGPUMemory();
}
