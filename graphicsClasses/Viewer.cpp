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
      _sceneRendering(scene),
      _menuRendering(menu, ftContent),
      _updatingSceneRendering( [this](size_t) {
          _sceneRendering.update();
      }),
      _updatingMenuRendering( [this](size_t) {
          _menuRendering.update();
      })
{
}

void Viewer::update() {
  _updatingSceneRendering.runTasks();
  _updatingMenuRendering.runTasks();
  _updatingSceneRendering.waitTasks();
  _updatingMenuRendering.waitTasks();
}

void Viewer::render() const {
  _sceneRendering.render();
  _menuRendering.render();
}
