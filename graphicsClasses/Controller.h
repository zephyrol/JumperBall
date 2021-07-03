//
// Controller.hpp
// JumperBallGraphic
//
// Created by S Morgenthaler on 25/05/2020.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <player/Player.h>
#include <scene/MapGenerator.h>
#include "scene/SceneRendering.h"
#include "Viewer.h"
#include "graphicMenu/MenuRendering.h"

class Controller {

public:

enum class Status { Pressed, Released };
enum class Button { Up, Down, Left, Right, Escape, Validate };
enum class ScreenDirection { North, South, East, West };


Controller();
~Controller();

void interactionButtons(const Button& button,
                        const Status& status);
void interactionMouse(const Status& status,
                      float posX, float posY);

void runController();
void waitController();

bool requestToLeave() const;


private:
ScreenDirection nearestDirection(float posX, float posY) const;
const std::shared_ptr <SceneRendering>& sceneRendering() const;
const std::shared_ptr <MenuRendering>& menuRendedering() const;
void renderRenderingEngine() const;
void renderMenuRendering() const;

void manageUp(const Status& status);
void manageDown(const Status& status);
void manageRight(const Status& status);
void manageLeft(const Status& status);
void manageEscape(const Status& status);
void manageValidateButton(const Status& status);
void runGame(size_t level);
void updateRenderingEngine();
void updateMenuRendering();

// Mouse/TouchPad/TouchScreen
void pressMouse(float posX, float posY);
void updateMouse(float posX, float posY);
void releaseMouse(float posX, float posY);
void manageValidateMouse();

std::shared_ptr<Viewer> createViewer() const;

FontTexturesGenerator::FTContent _ftContent;
Player _player;
std::shared_ptr <Menu> _menu;
std::map <Button, Status> _buttonsStatuts;


float _mouseCurrentXCoord;
float _mouseCurrentYCoord;
float _mousePressingXCoord;
float _mousePressingYCoord;
bool _mouseIsPressed;
bool _requestToLeave;

std::shared_ptr <Scene> _scene;
std::shared_ptr<Viewer> _viewer;

ParallelTask <void> _updatingSceneMenu;
ParallelTask <void> _updating;

};

#endif /* CONTROLLER_H */
