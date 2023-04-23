/*
 * File: Scene.h
 * Author: Morgenthaler S
 *
 * Created on 30 juin 2021, 21:22
 */

#ifndef SCENE_H
#define SCENE_H

#include <player/Player.h>
#include <scene/MapGenerator.h>
#include <scene/Ball.h>
#include <scene/Star.h>
#include "Camera.h"


class Scene;

using Scene_sptr = std::shared_ptr<Scene>;
using CstScene_sptr = std::shared_ptr<const Scene>;

class Scene {

public:

    enum class ActionKey {
        Up, Down, Left, Right, Validate, Nothing
    };

    Scene(const std::string &mapContent, float screenRatio, Player_sptr player, bool isUsingTouchScreen);

    void setNoAction();

    void setValidate();

    void setValidateMouse();

    void setRight();

    void setLeft();

    void setUp();

    void mouseSetUp();

    void setDown();

    std::string update();

    void updateScreenRatio(float ratio);

    CstMap_sptr getMap() const;

    CstBall_sptr getBall() const;

    std::shared_ptr<const Camera> getCamera() const;

    std::shared_ptr<const Star> getStar() const;

    std::shared_ptr<const Star> getStar2() const;

    float getRatio() const;

    bool isUsingTouchScreen() const;

private:
    Map_sptr _map;
    ActionKey _currentKey;
    const std::shared_ptr<Camera> _camera;
    const std::shared_ptr<Star> _star;
    const std::shared_ptr<Star> _star2;
    const Player_sptr _player;
    float _ratio;
    const bool _isUsingTouchScreen;

    bool isInGame() const;

};


#endif /* SCENE_H */
