/*
 * File: Scene.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 juin 2021, 21:22
 */
#include "Scene.h"

 Scene::Scene(size_t mapNumber):
  _map(MapGenerator::loadMap(mapNumber)),
  _ball(std::make_shared<Ball>(*_map)),
  _camera(std::make_shared<Camera>(*_map, *_ball)),
  _star(Star::createBlurStar(*_map))
 {
   
 }
 
 Player::Statut Scene::update(const Player::Statut& statut) {

   _ball->update();
   if (statut == Player::Statut::INMENU)
   {
     _camera->turnAroundMap();
   }
   else if (statut == Player::Statut::INGAME)
   {
     _camera->followBall();
   }
   else if (statut == Player::Statut::INTRANSITION)
   {
     _camera->approachBall();
   }
   _camera->update();

   if (_camera->getMovement() == Camera::Movement::FollowingBall) {
     return Player::Statut::INGAME;
   }

   return statut;
 }
 
 void Scene::doAction(const Scene::ActionKey& key) {
    switch (key) {
      case Scene::ActionKey::Up: {
        _ball->doAction(Ball::ActionRequest::GoStraightAhead);
        break;
      }
      case Scene::ActionKey::Down: {
        break;
      }
      case Scene::ActionKey::Right: {
        _ball->doAction(Ball::ActionRequest::TurnRight);
        break;
      }
      case Scene::ActionKey::Left: {
        _ball->doAction(Ball::ActionRequest::TurnLeft);
        break;
      }
      case Scene::ActionKey::Validate: {
        _ball->doAction(Ball::ActionRequest::Jump);
        break;
      }
      default: break;
    }
 }
 
 bool Scene::gameIsFinished() const {
    return _ball->stateOfLife() == Ball::StateOfLife::Dead;
 }
 
 std::shared_ptr<const Map> Scene::getMap() const {
    return _map; 
 }
 
 std::shared_ptr<const Ball> Scene::getBall() const {
    return _ball; 
 }
 
 std::shared_ptr<const Camera> Scene::getCamera() const {
    return _camera; 
 }
 
 std::shared_ptr<const Star> Scene::getStar() const {
    return _star; 
 }
 