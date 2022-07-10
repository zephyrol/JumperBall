/*
 * File: Scene.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 juin 2021, 21:22
 */
#include "Scene.h"

Scene::Scene(const std::string &mapContent, float screenRatio) :
    _map(MapGenerator::loadMap(mapContent)),
    _camera(std::make_shared<Camera>(*_map, screenRatio)),
    _star(Star::createBlurStar(*_map)),
    _star2(Star::createPurpleStar(*_map)) {
}

Player::Status Scene::update(const Player::Status &status, const Scene::ActionKey &key) {

    Ball::ActionRequest actionRequest;
    switch (key) {
        case Scene::ActionKey::Nothing: {
            actionRequest = Ball::ActionRequest::Nothing;
            break;
        }
        case Scene::ActionKey::Up: {
            actionRequest = Ball::ActionRequest::GoStraightAhead;
            break;
        }
        case Scene::ActionKey::Down: {
            actionRequest = Ball::ActionRequest::MoveCamera;
            break;
        }
        case Scene::ActionKey::Right: {
            actionRequest = Ball::ActionRequest::TurnRight;
            break;
        }
        case Scene::ActionKey::Left: {
            actionRequest = Ball::ActionRequest::TurnLeft;
            break;
        }
        case Scene::ActionKey::Validate: {
            actionRequest = Ball::ActionRequest::Jump;
            break;
        }
        default: {
            actionRequest = Ball::ActionRequest::Nothing;
            break;
        }
    }

    const auto updatingTime = JBTypesMethods::getTimePointMSNow();

    _map->update(updatingTime, actionRequest);
    _camera->update(updatingTime, status, actionRequest == Ball::ActionRequest::MoveCamera);

    if (_camera->getMovement() == Camera::Movement::FollowingBall) {
        return Player::Status::InGame;
    }

    return status;
}

bool Scene::gameIsLost() const {
    // TODO: call function gameIsLost in map
    return _map->getBall()->stateOfLife() == Ball::StateOfLife::Dead;
}

bool Scene::gameIsWon() const {
    // TODO: call function gameIsLost in map
    return _map->getBall()->stateOfLife() == Ball::StateOfLife::Winner;
}


CstMap_sptr Scene::getMap() const {
    return _map;
}

// TODO: useless method, remove it
std::shared_ptr<const Ball> Scene::getBall() const {
    return _map->getBall();
}

std::shared_ptr<const Camera> Scene::getCamera() const {
    return _camera;
}

std::shared_ptr<const Star> Scene::getStar() const {
    return _star;
}

std::shared_ptr<const Star> Scene::getStar2() const {
    return _star2;
}

void Scene::updateScreenRatio(float ratio) {
    _camera->setRatio(ratio);
}

