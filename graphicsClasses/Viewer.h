#ifndef VIEWER_H 
#define VIEWER_H 

#include <player/Player.h>
#include <scene/MapGenerator.h>
#include "scene/SceneRendering.h"
#include "graphicMenu/MenuRendering.h"

class Viewer {

Viewer(unsigned int resolutionX, unsigned int resolutionY);

std::shared_ptr <Map> _map;
std::shared_ptr <Ball> _ball;
std::shared_ptr <Camera> _camera;
std::shared_ptr <Star> _star;

std::shared_ptr <SceneRendering> _sceneRendering;
std::shared_ptr <MenuRendering> _menuRendering;

ParallelTask <void> _updatingScene;
ParallelTask <void> _updatingMenu;
ParallelTask <void> _updating;

};


#endif // VIEWER_H