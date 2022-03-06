/*
 * File: Viewer.h
 * Author: Morgenthaler S
 */

#ifndef VIEWER_H 
#define VIEWER_H 

#include <player/Player.h>
#include <scene/MapGenerator.h>
#include "scene/SceneRendering.h"
#include "graphicMenu/MenuRendering.h"

class Viewer {

public:
  Viewer(
      unsigned int resolutionX,
      unsigned int resolutionY,
      const Scene& scene,
      const Menu& menu,
      const FontTexturesGenerator::FTContent& ftContent,
      const JBTypes::FileContent& fileContent
  );

  void update();
  void render() const;
  void freeGPUMemory();

  const unsigned int _resolutionX;
  const unsigned int _resolutionY;

  SceneRendering _sceneRendering;
  MenuRendering _menuRendering;

};

#endif // VIEWER_H