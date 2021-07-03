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
      const FontTexturesGenerator::FTContent& ftContent
  );

  void update();
  void render() const;

  void swapFrames();

  SceneRendering _sceneRendering;
  MenuRendering _menuRendering;

  ParallelTask <void> _updatingSceneRendering;
  ParallelTask <void> _updatingMenuRendering;


};


#endif // VIEWER_H