//
// Controller.hpp
// JumperBallGraphic
//
// Created by S Morgenthaler on 25/05/2020.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "player/Player.h"
#include "scene/MapGenerator.h"
#include "scene/SceneRendering.h"
#include "Viewer.h"
#include "graphicMenu/PageRendering.h"

class Controller {

public:

    enum class Status {
        Pressed, Released
    };
    enum class Pointer {
        Mouse, TouchScreen
    };

    Controller(
        const size_t &screenWidth,
        const size_t &screenHeight,
        const JBTypes::FileContent &filesContent,
        const unsigned char *fontData,
        size_t fontDataSize,
        bool isUsingTouchScreen
    );

    void interactionButtons(const Button &button, const Status &status);

    void interactionMouse(const Status &status, float posX, float posY);

    void update();

    void render() const;

    bool requestToLeave() const;

    void resize(size_t screenWidth, size_t screenHeight);

private:

    void escapeAction();

    void runGame(size_t level);

    void updateMouse(float posX, float posY);

    void releaseMouse(float posX, float posY);

    void setValidateMouse();

    const Player_sptr _player;
    const std::shared_ptr<Menu> _menu;

    Status _currentEscapeStatus;

    const std::map<Button, std::function<void()> > _actionsWhenPressed;
    const std::map<ScreenDirection, std::function<void()> > _actionsMouseDirection;

    const JBTypes::FileContent _filesContent;

    bool _requestToLeave;

    std::shared_ptr<Scene> _scene;
    const std::shared_ptr<Viewer> _viewer;

};

#endif /* CONTROLLER_H */
