/*
 * File: Scene.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 juin 2021, 21:22
 */
#include "Scene.h"

Scene::Scene(const std::string &mapContent, float screenRatio, Player_sptr player) :
    _map(MapGenerator::loadMap(mapContent, player->getDoubleChronometer())),
    _currentKey(Scene::ActionKey::Nothing),
    _camera(std::make_shared<Camera>(*_map, screenRatio)),
    _star(Star::createBlurStar(*_map)),
    _star2(Star::createPurpleStar(*_map)),
    _player(std::move(player)),
    _ratio(screenRatio) {
}

std::string Scene::update() {

    const auto &status = _player->status();
    Ball::ActionRequest actionRequest;
    switch (_currentKey) {
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

    auto mapUpdateOutput = _map->update(actionRequest);
    _player->setRemainingTime(_map->remainingTime());

    _camera->update(status, actionRequest == Ball::ActionRequest::MoveCamera);

    if (_camera->getMovement() == Camera::Movement::FollowingBall) {
        _player->setAsInGame();
    }
    if (_player->status() == Player::Status::InGame) {
        if (_map->gameIsLost()) {
            _player->setAsLoser();
        }
        if (_map->gameIsWon()) {
            _player->setAsWinner(_map->getCurrentNumberOfCoins());
        }
    }
    return mapUpdateOutput;
}

CstMap_sptr Scene::getMap() const {
    return _map;
}

CstBall_sptr Scene::getBall() const {
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
    _ratio = ratio;
    _camera->setRatio(_ratio);
}

float Scene::getRatio() const {
    return _ratio;
}

void Scene::setValidate() {
    if (!isInGame()) {
        return;
    }
    _currentKey = Scene::ActionKey::Validate;
}

void Scene::setNoAction() {
    _currentKey = Scene::ActionKey::Nothing;
}

void Scene::setRight() {
    if (!isInGame()) {
        return;
    }
    _currentKey = Scene::ActionKey::Right;
}

void Scene::setLeft() {
    if (!isInGame()) {
        return;
    }
    _currentKey = Scene::ActionKey::Left;
}

void Scene::setUp() {
    if (!isInGame()) {
        return;
    }
    _currentKey = Scene::ActionKey::Up;
}

void Scene::setDown() {
    if (!isInGame()) {
        return;
    }
    _currentKey = Scene::ActionKey::Down;
}

void Scene::setValidateMouse() {
    if(!isInGame()) {
        return;
    }
    _currentKey = Scene::ActionKey::Validate;
}

bool Scene::isInGame() const {
    return _player->status() == Player::Status::InGame;
}

std::shared_ptr<const Player> Scene::getPlayer() const {
    return _player;
}


