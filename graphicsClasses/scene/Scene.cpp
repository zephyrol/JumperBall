/*
 * File: Scene.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 juin 2021, 21:22
 */
#include "Scene.h"

 Scene::Scene(size_t mapNumber):
  _map(MapGenerator::loadMap(mapNumber)),
  _camera(std::make_shared<Camera>(*_map)),
  _star(Star::createBlurStar(*_map))
 {
   
 }
 
 Player::Status Scene::update(const Player::Status& status, const Scene::ActionKey& key) {

   //_ball->update(); TODO update map (use interaction)

    Ball::ActionRequest actionRequest;
     switch (key) {
         case Scene::ActionKey::Up: {
             actionRequest = Ball::ActionRequest::GoStraightAhead;
             break;
         }
         case Scene::ActionKey::Down: {
             actionRequest = Ball::ActionRequest::Nothing;
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

     _map->update(JBTypesMethods::getTimePointMSNow(), actionRequest);

     if (status == Player::Status::INMENU) {
         _camera->turnAroundMap();
     }
     else if (status == Player::Status::INGAME) {
         _camera->followBall();
     }
     else if (status == Player::Status::INTRANSITION) {
         _camera->approachBall();
     }
     _camera->update();

     if (_camera->getMovement() == Camera::Movement::FollowingBall) {
         return Player::Status::INGAME;
     }

    return status;
 }

 bool Scene::gameIsFinished() const {
     // TODO: call function gameIsFinished in map
    return _map->getBall()->stateOfLife() == Ball::StateOfLife::Dead;
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
 
 vecCstSceneElement_sptr Scene::getSceneElements() const {
    CstSceneElement_sptr cstStar = _star;
    CstSceneElement_sptr cstCamera = _camera;
    CstSceneElement_sptr cstBall = _map->getBall();

    return { cstStar, cstBall, cstCamera };
 }
 