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
#include "Camera.h"

class Scene {

public:

enum class ActionKey { Up, Down, Left, Right, Validate };

Scene(size_t mapNumber);

Player::Statut update(const Player::Statut& statut);
void doAction( const ActionKey& key);

bool gameIsFinished() const;

std::shared_ptr<const Map> getMap() const;
std::shared_ptr<const Ball> getBall() const;
std::shared_ptr<const Camera> getCamera() const;
std::shared_ptr<const Star> getStar() const;

private:
const std::shared_ptr<Map> _map;
const std::shared_ptr<Ball> _ball;
const std::shared_ptr<Camera> _camera;
const std::shared_ptr<Star> _star;

};


#endif /* SCENE_H */
