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
#include "system/KeyboardKey.h"
#include "system/Mouse.h"
#include "system/DoubleChronometer.h"

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

    std::string update();

    void render() const;

    bool isRequestingLeaving() const;

    void resize(int screenWidth, int screenHeight);

    void stop();

    void resume();

    bool areMusicsActivated() const;

    bool areSoundsActivated() const;

private:

    void escapeAction();

    void runGame(size_t level);

    void setValidateMouse(float mouseX, float mouseY);

    const DoubleChronometer_sptr _doubleChronometer;
    const Player_sptr _player;

    const JBTypes::FileContent _filesContent;

    bool _requestToLeave;

    std::shared_ptr<Scene> _scene;
    const std::shared_ptr<Menu> _menu;
    const std::shared_ptr<Viewer> _viewer;

    KeyboardKey _keyboardKey;
    Mouse _mouse;
    std::vector<CstUpdateOutput_sptr> _outputs;
};

#endif /* CONTROLLER_H */
