/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.cpp
 * Author: seb
 * 
 * Created on 1 octobre 2019, 21:17
 */

#include "Ball.h"

Ball::Ball(): _currentBlockX(0),
        _currentBlockY(0),
        _currentBlockZ(0),
        _3DPosX(0.f),
        _3DPosY(0.f),
        _3DPosZ(0.f),
        _currentSide(JumperBallTypes::Direction::Up),
        _lookTowards(JumperBallTypes::Direction::North),
        _state(Ball::State::Staying),
        _mechanicsPattern(),
        _timeAction(),
        _map(nullptr)
{
}

/*Ball::Ball(const Ball& orig):
 {
 }*/

Ball::~Ball() {
}


void Ball::turnLeft() noexcept {

    switch (_currentSide) {
        case JumperBallTypes::Direction::North:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North: break;
                case JumperBallTypes::Direction::South: break;
                case JumperBallTypes::Direction::East: 
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::South:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North: break;
                case JumperBallTypes::Direction::South: break;
                case JumperBallTypes::Direction::East: 
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::East:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::East: break;
                case JumperBallTypes::Direction::West: break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::West:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::East: break;
                case JumperBallTypes::Direction::West: break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::Up:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                case JumperBallTypes::Direction::East:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                case JumperBallTypes::Direction::Up: break;
                case JumperBallTypes::Direction::Down: break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::Down:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                case JumperBallTypes::Direction::East:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                case JumperBallTypes::Direction::Up: break;
                case JumperBallTypes::Direction::Down: break;
                default : break;
            }
            break;
        default :
            break;
    }
}


void Ball::turnRight() noexcept {
    switch (_currentSide) {
        case JumperBallTypes::Direction::North:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North: break;
                case JumperBallTypes::Direction::South: break;
                case JumperBallTypes::Direction::East: 
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::South:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North: break;
                case JumperBallTypes::Direction::South: break;
                case JumperBallTypes::Direction::East: 
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::East:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::East: break;
                case JumperBallTypes::Direction::West: break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::West:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::Up; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::Down; break;
                case JumperBallTypes::Direction::East: break;
                case JumperBallTypes::Direction::West: break;
                case JumperBallTypes::Direction::Up:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                case JumperBallTypes::Direction::Down:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::Up:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                case JumperBallTypes::Direction::East:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                case JumperBallTypes::Direction::Up: break;
                case JumperBallTypes::Direction::Down: break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::Down:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    _lookTowards =  JumperBallTypes::Direction::West; break;
                case JumperBallTypes::Direction::South:
                    _lookTowards =  JumperBallTypes::Direction::East; break;
                case JumperBallTypes::Direction::East:
                    _lookTowards =  JumperBallTypes::Direction::North; break;
                case JumperBallTypes::Direction::West:
                    _lookTowards =  JumperBallTypes::Direction::South; break;
                case JumperBallTypes::Direction::Up: break;
                case JumperBallTypes::Direction::Down: break;
                default : break;
            }
            break;
        default :
            break;
    }
}

void Ball::updatePosition() noexcept {
  
    constexpr float offsetRealPosition = 0.5f;
    auto now = std::chrono::system_clock::now();
    static_cast<void>(now);
    if (_state == Ball::State::Staying) {
        _3DPosX = static_cast<float> (_currentBlockX) + offsetRealPosition;
        _3DPosY = static_cast<float> (_currentBlockY) + offsetRealPosition;
        _3DPosZ = static_cast<float> (_currentBlockZ) + offsetRealPosition;
    } else if ( _state == Ball::State::Moving){

    } else if ( _state == Ball::State::Jumping){

    }

    
}


void Ball::goStraightOn() noexcept {
    
    int inFrontOfX = _currentBlockX;
    int inFrontOfY = _currentBlockY;
    int inFrontOfZ = _currentBlockZ;
    
    int leftX = _currentBlockX;
    int leftY = _currentBlockY;
    int leftZ = _currentBlockZ;
    
    int rightX = _currentBlockX;
    int rightY = _currentBlockY;
    int rightZ = _currentBlockZ;
    
    int aboveX = _currentBlockX;
    int aboveY = _currentBlockY;
    int aboveZ = _currentBlockZ;
    
    switch (_currentSide) {
        case JumperBallTypes::Direction::North:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North: break;
                case JumperBallTypes::Direction::South: break;
                case JumperBallTypes::Direction::East: 
                    ++inFrontOfX; ++aboveX; --aboveZ; --leftY; ++rightY; break;
                case JumperBallTypes::Direction::West:
                    --inFrontOfX; --aboveX; --aboveZ; ++leftY; --rightY; break;
                case JumperBallTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; --aboveZ; ++leftX; --rightX; break;
                case JumperBallTypes::Direction::Down:
                    --inFrontOfY; --aboveY; --aboveZ; --leftX; ++rightX; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::South:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North: break;
                case JumperBallTypes::Direction::South: break;
                case JumperBallTypes::Direction::East: 
                    ++inFrontOfX; ++aboveX; ++aboveZ; ++leftY; --rightY; break;
                case JumperBallTypes::Direction::West:
                    --inFrontOfX; --aboveX; ++aboveZ; --leftY; ++rightY; break;
                case JumperBallTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; ++aboveZ; --leftX; ++rightX; break;
                case JumperBallTypes::Direction::Down:
                    --inFrontOfY; --aboveY; ++aboveZ; ++leftX; --rightX; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::East:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; ++aboveX; ++leftY; --rightY; break;
                case JumperBallTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; ++aboveX; --leftY; ++rightY; break;
                case JumperBallTypes::Direction::East: break;
                case JumperBallTypes::Direction::West: break;
                case JumperBallTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; ++aboveX; --leftZ; ++rightZ; break;
                case JumperBallTypes::Direction::Down:
                    --inFrontOfY; --aboveY; ++aboveX; ++leftZ; --rightZ; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::West:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; --aboveX; --leftY; ++rightY; break;
                case JumperBallTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; --aboveX; ++leftY; --rightY; break;
                case JumperBallTypes::Direction::East: break;
                case JumperBallTypes::Direction::West: break;
                case JumperBallTypes::Direction::Up:
                    ++inFrontOfY; ++aboveY; --aboveX; ++leftZ; --rightZ; break;
                case JumperBallTypes::Direction::Down:
                    --inFrontOfY; --aboveY; --aboveX; --leftZ; ++rightZ; break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::Up:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; ++aboveY; --leftX; ++rightX; break;
                case JumperBallTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; ++aboveY; ++leftX; --rightX; break;
                case JumperBallTypes::Direction::East:
                    ++inFrontOfX; ++aboveX; ++aboveY; --leftZ; ++rightZ; break;
                case JumperBallTypes::Direction::West:
                    --inFrontOfX; --aboveX; ++aboveY; ++leftZ; --rightZ; break;
                case JumperBallTypes::Direction::Up: break;
                case JumperBallTypes::Direction::Down: break;
                default : break;
            }
            break;
        case JumperBallTypes::Direction::Down:
            switch (_lookTowards) {
                case JumperBallTypes::Direction::North:
                    --inFrontOfZ; --aboveZ; --aboveY; ++leftX; --rightX; break;
                case JumperBallTypes::Direction::South:
                    ++inFrontOfZ; ++aboveZ; --aboveY; --leftX; ++rightX; break;
                case JumperBallTypes::Direction::East:
                    ++inFrontOfX; ++aboveX; --aboveY; ++leftZ; --rightZ; break;
                case JumperBallTypes::Direction::West:
                    --inFrontOfX; --aboveX; --aboveY; --leftZ; ++rightZ; break;
                case JumperBallTypes::Direction::Up: break;
                case JumperBallTypes::Direction::Down: break;
                default : break;
            }
        default :
            break;
    }

    if (_map->map3DData(aboveX,aboveY,aboveZ)) {
        _currentBlockX = aboveX; _currentBlockY = aboveY; 
        _currentBlockZ = aboveZ; 
    } 
    else if (_map->map3DData(inFrontOfX,inFrontOfY,inFrontOfZ)) {
        _currentBlockX = inFrontOfX; _currentBlockY = inFrontOfY; 
        _currentBlockZ = inFrontOfZ; 
    } 
    else if (!_map->map3DData(leftX,leftY,leftZ) && 
            !_map->map3DData(rightX,rightY,rightZ)) {
        
        JumperBallTypes::Direction sideBeforeMovement = _currentSide;
        _currentSide = _lookTowards;
        
        switch (sideBeforeMovement) {
            case JumperBallTypes::Direction::North:
                _lookTowards = JumperBallTypes::Direction::South;
                break;
            case JumperBallTypes::Direction::South:
                _lookTowards = JumperBallTypes::Direction::North;
                break;
            case JumperBallTypes::Direction::East:
                _lookTowards = JumperBallTypes::Direction::West;
                break;
            case JumperBallTypes::Direction::West:
                _lookTowards = JumperBallTypes::Direction::East;
                break;
            case JumperBallTypes::Direction::Up:
                _lookTowards = JumperBallTypes::Direction::Down;
                break;
            case JumperBallTypes::Direction::Down:
                _lookTowards = JumperBallTypes::Direction::Up;
                break;
            default :
                break;
        }
    }
    else stay();
}


void Ball::jump() noexcept {
    _state = Ball::State::Jumping;
}

void Ball::stay() noexcept {
    _state = Ball::State::Staying;
}

void Ball::setTimeActionNow() noexcept {
    _timeAction = std::chrono::system_clock::now();
}


Ball::AnswerRequest Ball::doAction(Ball::ActionRequest action) {
    Ball::AnswerRequest answer (Ball::AnswerRequest::Accepted);
    switch (action) {
        case Ball::ActionRequest::GoStraightOn:
            if (_state == Ball::State::Staying) {
                _state = Ball::State::Moving;
                goStraightOn();
                
            }
            else answer = Ball::AnswerRequest::Rejected;
            break;
        case Ball::ActionRequest::TurnLeft:
            if (_state == Ball::State::Staying) {
                _state = Ball::State::Moving;
                turnLeft();
            }
            else answer = Ball::AnswerRequest::Rejected;
            break;
        case Ball::ActionRequest::TurnRight:
            if (_state == Ball::State::Staying) {
                _state = Ball::State::Moving;
                turnRight();
            }
            else answer = Ball::AnswerRequest::Rejected;
            break;
        case Ball::ActionRequest::Jump:
            if (_state == Ball::State::Staying) {
                _state = Ball::State::Jumping;
                jump();
            }
            else answer = Ball::AnswerRequest::Rejected;
            break;
        default :
            break;
    }

    if (answer == Ball::AnswerRequest::Accepted)
        setTimeActionNow();
    
    return answer;
}



Ball::AnswerRequest Ball::isFallingIntersectionBlock() noexcept {

  Ball::AnswerRequest answer = Ball::AnswerRequest::Rejected; 
  if ( _state == Ball::State::Jumping ) {
      std::chrono::time_point<std::chrono::system_clock> timeNow = 
          std::chrono::system_clock::now();
      auto timeJump = timeNow - _timeAction;

       if ( _mechanicsPattern.getVelocity(0.f).y < 0 ) {
          answer = Ball::AnswerRequest::Accepted; 

       }
    
  }
  return answer;
}

