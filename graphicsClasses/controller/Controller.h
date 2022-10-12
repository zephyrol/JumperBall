//
// Controller.hpp
// JumperBallGraphic
//
// Created by S.Morgenthaler on 25/05/2020.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player/Player.h"
#include "scene/MapGenerator.h"
#include "scene/SceneRendering.h"
#include "Viewer.h"
#include "graphicMenu/PageRendering.h"
#include "system/KeyboardKey.h"
#include "system/Mouse.h"

class Controller {

public:

    Controller(
        const size_t &screenWidth,
        const size_t &screenHeight,
        const JBTypes::FileContent &filesContent,
        const unsigned char *fontData,
        size_t fontDataSize,
        bool isUsingTouchScreen
    );

    void interactionButtons(const KeyboardKey::Button &button, const KeyboardKey::Status &status);

    void pressMouse(float posX, float posY);

    void releaseMouse();

    void update();

    void render() const;

    bool requestToLeave() const;

    void resize(size_t screenWidth, size_t screenHeight);

    void stop();

    void resume();

private:

    void escapeAction();

    void runGame(size_t level);

    void setValidateMouse(float mouseX, float mouseY);

    const Chronometer_sptr _chronometer;
    const Player_sptr _player;
    const std::shared_ptr<Menu> _menu;

    const JBTypes::FileContent _filesContent;

    bool _requestToLeave;

    std::shared_ptr<Scene> _scene;
    const std::shared_ptr<Viewer> _viewer;

    KeyboardKey _keyboardKey;
    Mouse _mouse;

};

#endif /* CONTROLLER_H */
