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
#include "graphicMenu/PageRendering.h"

class Controller {

public:

enum class Status { Pressed, Released };
enum class Button { Up, Down, Left, Right, Escape, Validate };
enum class ScreenDirection { North, South, East, West };
enum class Pointer { Mouse, TouchScreen };

Controller(
    const size_t& screenWidth,
    const size_t& screenHeight,
    const JBTypes::FileContent& filesContent,
    const unsigned char* fontData,
    size_t fontDataSize,
    bool isUsingTouchScreen
);

void interactionButtons(const Button& button,
                        const Status& status);
void interactionMouse(const Status& status,
                      float posX, float posY);

void update();
void render() const;

bool requestToLeave() const;

void resize(size_t screenWidth, size_t screenHeight);

private:

void updateViewer();
void updateSceneMenu();

ScreenDirection nearestDirection(float posX, float posY) const;

static float computeDistance(float x0, float y0, float x1, float y1);

void setUp(const Status& status);
void setDown(const Status& status);
void setRight(const Status& status);
void setLeft(const Status& status);
void setEscape(const Status& status);
void setValidateButton(const Status& status);
void runGame(size_t level);

// Mouse/TouchPad/TouchScreen
void pressMouse(float posX, float posY);
void updateMouse(float posX, float posY);
void releaseMouse(float posX, float posY);
void setValidateMouse();

Player _player;
std::shared_ptr <Menu> _menu;
std::map <Button, Status> _buttonsStatus;
Scene::ActionKey _currentKey;

const JBTypes::FileContent _filesContent;
const bool _isUsingTouchScreen;
float _ratio;

float _mousePressingXCoord;
float _mousePressingYCoord;
float _mouseCurrentXCoord;
float _mouseCurrentYCoord;
float _mousePreviousXCoord;
float _mousePreviousYCoord;
JBTypes::timePointMs _mouseUpdatingTime;
JBTypes::timePointMs _mousePressTime;
std::shared_ptr<const Controller::ScreenDirection> _currentMovementDir;
bool _mouseIsPressed;
bool _requestToLeave;

std::shared_ptr <Scene> _scene;
std::shared_ptr<Viewer> _viewer;

};

#endif /* CONTROLLER_H */
