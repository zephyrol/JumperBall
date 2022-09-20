/*
 * File: Scene.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 juin 2021, 21:22
 */
#include "Scene.h"

Scene::Scene(const std::string &mapContent, float screenRatio, Player_sptr player, bool isUsingTouchScreen) :
    _map(MapGenerator::loadMap(mapContent)),
    _currentKey(Scene::ActionKey::Nothing),
    _camera(std::make_shared<Camera>(*_map, screenRatio)),
    _star(Star::createBlurStar(*_map)),
    _star2(Star::createPurpleStar(*_map)),
    _player(std::move(player)),
    _ratio(screenRatio),
    _isUsingTouchScreen(isUsingTouchScreen) {
}

void Scene::update() {

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

    const auto updatingTime = JBTypesMethods::getTimePointMSNow();

    _map->update(updatingTime, actionRequest);
    _camera->update(updatingTime, status, actionRequest == Ball::ActionRequest::MoveCamera);

    _currentKey = Scene::ActionKey::Nothing;

    if (_camera->getMovement() == Camera::Movement::FollowingBall) {
        _player->status(Player::Status::InGame);
    }
    if (_player->status() != Player::Status::InGame) {
        return;
    }
    if (_map->gameIsLost()) {
        _player->setAsLoser();
    }
    if (_map->gameIsWon()) {
        _player->setAsWinner();
    }

}

CstMap_sptr Scene::getMap() const {
    return _map;
}

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
    _currentKey = _isUsingTouchScreen ? Scene::ActionKey::Validate : Scene::ActionKey::Up;
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
    if(!_isUsingTouchScreen) {
        _currentKey = Scene::ActionKey::Validate;
    }
}

bool Scene::isInGame() const {
    return _player->status() == Player::Status::InGame;
}

bool Scene::isUsingTouchScreen() const {
    return _isUsingTouchScreen;
}

void Scene::mouseSetUp() {
    if(!isInGame() || !_isUsingTouchScreen) {
        return;
    }
    _currentKey = Scene::ActionKey::Up;
}


