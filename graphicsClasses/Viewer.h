#ifndef VIEWER_H
#define VIEWER_H

#include <player/Player.h>
#include <scene/MapGenerator.h>
#include "scene/SceneRendering.h"
#include "graphicMenu/MenuRendering.h"

class Viewer {
Viewer(
    unsigned int resolutionX,
    unsigned int resolutionY,
    const Map& _map,
    const Ball& _ball,
    const Camera& _camera,
    const Star& _star,
    const Menu& menu,
    const FontTexturesGenerator::FTContent& ftContent
    );

unsigned int _resolutionX;
unsigned int _resolutionY;
const Map& _map;
const Ball& _ball;
const Camera& _camera;
const Star& _star;
const Menu& _menu;
const FontTexturesGenerator::FTContent& _ftContent;

void update();
void render() const;

std::shared_ptr <SceneRendering> _sceneRendering;
std::shared_ptr <MenuRendering> _menuRendering;
};

#endif // VIEWER_H