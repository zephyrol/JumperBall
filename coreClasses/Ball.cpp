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
#include <iostream>
#include <string>
#include <algorithm>
            
Ball::Ball(const Map& map): 
        _currentBlockX(map.beginX()),
        _currentBlockY(map.beginY()),
        _currentBlockZ(map.beginZ()),
        _3DPosX(0.f),
        _3DPosY(0.f),
        _3DPosZ(0.f),
        _currentSide(JumperBallTypes::Direction::Up),
        _lookTowards(JumperBallTypes::Direction::North),
        _state(Ball::State::Staying),
        _map(map),
        _mechanicsPattern(),
        _timeAction(),
        _shocks{} {
       
}

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

std::array<float,3> Ball::get3DPosition() const noexcept {
    return std::array<float,3> {_3DPosX,_3DPosY,_3DPosZ};
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

    if (_map.map3DData(aboveX,aboveY,aboveZ)) {
        _currentBlockX = aboveX; _currentBlockY = aboveY; 
        _currentBlockZ = aboveZ; 

        JumperBallTypes::Direction lookTowardsBeforeMovement = _lookTowards;

        _lookTowards = _currentSide;
        
        switch (lookTowardsBeforeMovement) {
            case JumperBallTypes::Direction::North:
                _currentSide = JumperBallTypes::Direction::South;
                break;
            case JumperBallTypes::Direction::South:
                _currentSide = JumperBallTypes::Direction::North;
                break;
            case JumperBallTypes::Direction::East:
                _currentSide= JumperBallTypes::Direction::West;
                break;
            case JumperBallTypes::Direction::West:
                _currentSide= JumperBallTypes::Direction::East;
                break;
            case JumperBallTypes::Direction::Up:
                _currentSide= JumperBallTypes::Direction::Down;
                break;
            case JumperBallTypes::Direction::Down:
                _currentSide= JumperBallTypes::Direction::Up;
                break;
            default :
                break;
        }

    } 
    else if (_map.map3DData(inFrontOfX,inFrontOfY,inFrontOfZ)) {
        _currentBlockX = inFrontOfX; _currentBlockY = inFrontOfY; 
        _currentBlockZ = inFrontOfZ; 
    } 
    else if (!_map.map3DData(leftX,leftY,leftZ) && 
            !_map.map3DData(rightX,rightY,rightZ)) {
        
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
    //updatePosition();
}

void Ball::jump() noexcept {
    _state = Ball::State::Jumping;
    setTimeActionNow();
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
                goStraightOn();
                
            }
            else answer = Ball::AnswerRequest::Rejected;
            break;
        case Ball::ActionRequest::TurnLeft:
            if (_state == Ball::State::Staying) {
                turnLeft();
            }
            else answer = Ball::AnswerRequest::Rejected;
            break;
        case Ball::ActionRequest::TurnRight:
            if (_state == Ball::State::Staying) {
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

    //updatePosition(); 

    if (answer == Ball::AnswerRequest::Accepted)
        setTimeActionNow();
    return answer;
}


Ball::AnswerRequest Ball::isFallingIntersectionBlock() noexcept {

    Ball::AnswerRequest answer = Ball::AnswerRequest::Rejected; 
    if ( _state == Ball::State::Jumping ) {
        
        
        const float fDifference = getTimeSecondsSinceAction();
        
        if ( _mechanicsPattern.getVelocity(fDifference).y < 0 ) {
            answer = Ball::AnswerRequest::Accepted; 
            
            auto positionBlockPtr = intersectBlock(_3DPosX,_3DPosY, _3DPosZ);
            if (positionBlockPtr) {

                int aboveNearX =  _currentBlockX;
                int aboveNearY =  _currentBlockY;
                int aboveNearZ =  _currentBlockZ;
                int aboveFarX =   _currentBlockX;
                int aboveFarY =   _currentBlockY;
                int aboveFarZ =   _currentBlockZ;

                bool frontalShock = false;

                switch (_currentSide) {
                    case JumperBallTypes::Direction::North:
                        --aboveNearZ; --aboveFarZ;   break;
                    case JumperBallTypes::Direction::South:
                        ++aboveNearZ; ++aboveFarZ;   break;
                    case JumperBallTypes::Direction::East: 
                        ++aboveNearX; ++aboveFarX;   break;
                    case JumperBallTypes::Direction::West:
                        --aboveNearX; --aboveFarX;   break;
                    case JumperBallTypes::Direction::Up:
                        ++aboveNearY; ++aboveFarY;   break;
                    case JumperBallTypes::Direction::Down:
                        --aboveNearY; --aboveFarY;   break;
                    default : break;
                }
                
                switch (_lookTowards) {
                    case JumperBallTypes::Direction::North:
                        --aboveNearZ; aboveFarZ -= 2 ;   break;
                    case JumperBallTypes::Direction::South:
                        ++aboveNearZ; aboveFarZ += 2;   break;
                    case JumperBallTypes::Direction::East: 
                        ++aboveNearX; aboveFarX += 2;   break;
                    case JumperBallTypes::Direction::West:
                        --aboveNearX; aboveFarX -= 2;   break;
                    case JumperBallTypes::Direction::Up:
                        ++aboveNearY; aboveFarY += 2;   break;
                    case JumperBallTypes::Direction::Down:
                        --aboveNearY; aboveFarY -= 2;   break;
                    default : break;
                }
                
                std::shared_ptr<Block> blockNear = 
                              _map.map3DData(aboveNearX,aboveNearY,aboveNearZ);

                std::shared_ptr<Block> blockFar = 
                              _map.map3DData(aboveFarX,aboveFarY,aboveFarZ);

                std::array<unsigned int,3> shockPosition;
                if (blockNear) {
                  if (aboveNearX == positionBlockPtr->at(0) &&
                      aboveNearY == positionBlockPtr->at(1) &&
                      aboveNearZ == positionBlockPtr->at(2))  {
                        frontalShock = true;               
                      
                      }
                }
                else if (blockFar) {
                  if ( aboveFarX == positionBlockPtr->at(0) &&
                       aboveFarY == positionBlockPtr->at(1) &&
                       aboveFarZ == positionBlockPtr->at(2))  {
                        frontalShock = true;               
                        
                      }
                }
                if (frontalShock) {
                    shock s (shockPosition);
					
					_shocks.clear();
                    _mechanicsPattern.timesShock({});
                    
                    _state = Ball::State::Staying;
					
                    /*auto it = std::find(_shocks.begin(),_shocks.end(),s);
                    if ( it == _shocks.end()) {
                        _shocks.push_back(s);
                        std::vector<float> shocks=
                                                _mechanicsPattern.timesShock();
                        shocks.push_back(getTimeSecondsSinceAction());
                        _mechanicsPattern.timesShock(shocks);
                    }*/
                    
                }
                else {
                    _shocks.clear();
                    _mechanicsPattern.timesShock({});
                    _currentBlockX  = positionBlockPtr->at(0) ;
                    _currentBlockY  = positionBlockPtr->at(1) ;
                    _currentBlockZ  = positionBlockPtr->at(2) ;
                    _state = Ball::State::Staying;
                }
                //update();
            }
            else {
                /*_3DPosX = pos3D.at(0);
                 _3DPosY = pos3D.at(1);
                 _3DPosZ = pos3D.at(2);*/
            }
        }
        answer = Ball::AnswerRequest::Accepted; 
    }
    return answer;
}

float Ball::distanceBehindBall() const 
{
    constexpr float offsetCenterBlock = 0.5f;
    float distance;
    switch (_lookTowards) {
        case JumperBallTypes::Direction::North:
            distance = _3DPosZ- (static_cast<float>(_currentBlockZ)
                              + offsetCenterBlock);
            break;
        case JumperBallTypes::Direction::South:
            distance = _3DPosZ- (static_cast<float>(_currentBlockZ)
                              + offsetCenterBlock);
            break;
        case JumperBallTypes::Direction::East:
            distance = _3DPosX- (static_cast<float>(_currentBlockX)
                              + offsetCenterBlock);
            break;
        case JumperBallTypes::Direction::West:
            distance = _3DPosX- (static_cast<float>(_currentBlockX)
                              + offsetCenterBlock);
            break;
        case JumperBallTypes::Direction::Up:
            distance = _3DPosY- (static_cast<float>(_currentBlockY)
                              + offsetCenterBlock);
            break;
        case JumperBallTypes::Direction::Down:
            distance = _3DPosY- (static_cast<float>(_currentBlockY)
                              + offsetCenterBlock);
            break;
        default :
            break;
    }

    if (distance < 0 ) distance = -distance;
    return offsetCenterBlock + distance;
}

std::shared_ptr<const std::vector<int> > Ball::intersectBlock(float x, 
                                                              float y, 
                                                              float z) const {
    
    std::shared_ptr<const std::vector<int> > blockIntersected = nullptr;
    
    const float offsetBlockPosition = _mechanicsPattern.radiusBall;
    int xInteger;
    int yInteger;
    int zInteger;
    float xIntersection = x;
    float yIntersection = y;
    float zIntersection = z;
    
    switch (_currentSide) {
        case JumperBallTypes::Direction::North:
            zIntersection += offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::South:
            zIntersection -= offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::East:
            xIntersection -= offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::West:
            xIntersection += offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::Up:
            yIntersection -= offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::Down:
            yIntersection += offsetBlockPosition ;
            break;
        default :
            break;
    }
    
    xInteger = static_cast<int> (xIntersection);
    yInteger = static_cast<int> (yIntersection);
    zInteger = static_cast<int> (zIntersection);
    
    if (_map.map3DData(xInteger, yInteger, zInteger)) 
        blockIntersected = std::make_shared<const std::vector<int> > (
                std::initializer_list<int> ({xInteger,yInteger,zInteger}));
    else  {

        xIntersection = x;
        yIntersection = y;
        zIntersection = z;
        
      /*  switch (_lookTowards) {
            case JumperBallTypes::Direction::North:
                zIntersection -= offsetBlockPosition ;
                break;
            case JumperBallTypes::Direction::South:
                zIntersection += offsetBlockPosition ;
                break;
            case JumperBallTypes::Direction::East:
                xIntersection += offsetBlockPosition ;
                break;
            case JumperBallTypes::Direction::West:
                xIntersection -= offsetBlockPosition ;
                break;
            case JumperBallTypes::Direction::Up:
                yIntersection += offsetBlockPosition ;
                break;
            case JumperBallTypes::Direction::Down:
                yIntersection -= offsetBlockPosition ;
                break;
            default :
                break;
        }*/
        xInteger = static_cast<int> (xIntersection);
        yInteger = static_cast<int> (yIntersection);
        zInteger = static_cast<int> (zIntersection);
        if (_map.map3DData(xInteger, yInteger, zInteger)) 
            blockIntersected = std::make_shared<const std::vector<int> > (
                    std::initializer_list<int> ({xInteger,yInteger,zInteger}));
    }
    
    return blockIntersected;
}

Ball::timePointMs Ball::getTimeActionMs() const noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds>
                                                      (_timeAction);
}

Ball::timePointMs Ball::getTimePointMSNow() noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds> 
                                    (std::chrono::system_clock::now());
}

std::array<float,3> Ball::P2DTo3D(ClassicalMechanics::physics2DVector p2D) const
{

    float x = static_cast<float> (_currentBlockX + 0.5f);
    float y = static_cast<float> (_currentBlockY + 0.5f);
    float z = static_cast<float> (_currentBlockZ + 0.5f);

    const float offsetRealPosition = 0.5f + _mechanicsPattern.radiusBall;

    switch (_currentSide) {
        case JumperBallTypes::Direction::North:
            z -= offsetRealPosition + p2D.y;
            break;
        case JumperBallTypes::Direction::South:
            z += offsetRealPosition + p2D.y;
            break;
        case JumperBallTypes::Direction::East:
            x += offsetRealPosition + p2D.y;
            break;
        case JumperBallTypes::Direction::West:
            x -= offsetRealPosition + p2D.y;
            break;
        case JumperBallTypes::Direction::Up:
            y += offsetRealPosition + p2D.y;
            break;
        case JumperBallTypes::Direction::Down:
            y -= offsetRealPosition + p2D.y;
            break;
        default :
            break;
    }

    switch (_lookTowards) {
        case JumperBallTypes::Direction::North:
            z -=  p2D.x;
            break;
        case JumperBallTypes::Direction::South:
            z += p2D.x;
            break;
        case JumperBallTypes::Direction::East:
            x += p2D.x;
            break;
        case JumperBallTypes::Direction::West:
            x -= p2D.x;
            break;
        case JumperBallTypes::Direction::Up:
            y += p2D.x;
            break;
        case JumperBallTypes::Direction::Down:
            y -= p2D.x;
            break;
        default :
            break;
    }

    return std::array<float,3> {x,y,z};
}

float Ball::getRadius() const {
    return _mechanicsPattern.radiusBall;
}

JumperBallTypes::Direction Ball::currentSide() const {
    return _currentSide;
}

JumperBallTypes::Direction Ball::lookTowards() const {
    return _lookTowards;
}

float Ball::getTimeActionSecondsFloat() const noexcept {

    const timePointMs timeActionMs           = getTimeActionMs(); 
    
    const durationMs timeActionSinceEpoch    = timeActionMs.time_since_epoch();
    
    const std::chrono::duration<float> durationFloat = timeActionSinceEpoch;
    std::cout<< "timeaction float" << durationFloat.count()/1000.f << std::endl;
    return durationFloat.count() / 1000.f ;
}

float Ball::getTimeNowSecondsFloat() noexcept {

    const timePointMs timeNowMs           = getTimePointMSNow(); 
    
    const durationMs timeNowSinceEpoch    = timeNowMs.time_since_epoch();
    
    const std::chrono::duration<float> durationFloat = timeNowSinceEpoch;
    
    std::cout<< "timenow time" << durationFloat.count()/1000.f << std::endl;
    return durationFloat.count() / 1000.f ;
}

float Ball::getTimeSecondsSinceAction() const noexcept{

    const timePointMs timeNowMs              = getTimePointMSNow(); 
    const durationMs timeNowSinceEpoch       = timeNowMs.time_since_epoch();
    
    const timePointMs timeActionMs           = getTimeActionMs(); 
    const durationMs timeActionSinceEpoch    = timeActionMs.time_since_epoch();
    
    const durationMs difference = timeNowSinceEpoch - timeActionSinceEpoch;
    const std::chrono::duration<float> durationFloatDifference = difference;
    const float fDifference = durationFloatDifference.count();

    return fDifference;
}

std::array<float, 3> Ball::lookTowardsAsVector() const {
    std::array<float,3> lookVec3 {0.f,0.f,0.f};
    switch (_lookTowards) {
        case JumperBallTypes::Direction::North:
            lookVec3.at(2) = -1.f;
            break;
        case JumperBallTypes::Direction::South:
            lookVec3.at(2) = 1.f;
            break;
        case JumperBallTypes::Direction::East:
            lookVec3.at(0) = 1.f;
            break;
        case JumperBallTypes::Direction::West:
            lookVec3.at(0) = -1.f;
            break;
        case JumperBallTypes::Direction::Up:
            lookVec3.at(1) = 1.f;
            break;
        case JumperBallTypes::Direction::Down:
            lookVec3.at(1) = -1.f;
            break;
        default :
            break;
    }

    return lookVec3;

}

void Ball::update() noexcept{

    constexpr float offsetPosition = 0.5f + _mechanicsPattern.radiusBall;

    std::array<float,3> position3D {0.f,0.f,0.f};

    if (_state == Ball::State::Staying) {
        
        float x = static_cast<float> (_currentBlockX + 0.5f);
        float y = static_cast<float> (_currentBlockY + 0.5f);
        float z = static_cast<float> (_currentBlockZ + 0.5f);
        
        
        switch (_currentSide) {
            case JumperBallTypes::Direction::North:
                z -= offsetPosition ;
                break;
            case JumperBallTypes::Direction::South:
                z += offsetPosition ;
                break;
            case JumperBallTypes::Direction::East:
                x += offsetPosition ;
                break;
            case JumperBallTypes::Direction::West:
                x -= offsetPosition ;
                break;
            case JumperBallTypes::Direction::Up:
                y += offsetPosition ;
                break;
            case JumperBallTypes::Direction::Down:
                y -= offsetPosition ;
                break;
            default :
                break;
        }
        position3D = {x,y,z};
        _3DPosX = position3D.at(0);
        _3DPosY = position3D.at(1);
        _3DPosZ = position3D.at(2);
    } else if ( _state == Ball::State::Moving){

    } else if ( _state == Ball::State::Jumping){
        ClassicalMechanics::physics2DVector pos2D = 
          _mechanicsPattern.getPosition(getTimeSecondsSinceAction());
        std::array<float,3> relativePositionJump = P2DTo3D(pos2D);
        
        position3D = { relativePositionJump.at(0),
                       relativePositionJump.at(1),
                       relativePositionJump.at(2),
                   };
        _3DPosX = position3D.at(0);
        _3DPosY = position3D.at(1);
        _3DPosZ = position3D.at(2);

        isFallingIntersectionBlock();
    }

}