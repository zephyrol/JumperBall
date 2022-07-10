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

class Scene {

public:

    enum class ActionKey {
        Up, Down, Left, Right, Validate, Nothing
    };

    Scene(const std::string &mapContent, float screenRatio);

    Player::Status update(const Player::Status &status, const ActionKey &key);

    void updateScreenRatio(float ratio);

    bool gameIsLost() const;

    bool gameIsWon() const;

    CstMap_sptr getMap() const;

    std::shared_ptr<const Ball> getBall() const;

    std::shared_ptr<const Camera> getCamera() const;

    std::shared_ptr<const Star> getStar() const;

    std::shared_ptr<const Star> getStar2() const;

private:
    const Map_sptr _map;
    const std::shared_ptr<Camera> _camera;
    const std::shared_ptr<Star> _star;
    const std::shared_ptr<Star> _star2;

};


#endif /* SCENE_H */
