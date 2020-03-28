/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.cpp
 * Author: Morgenthaler S
 * 
 * Created on 1 octobre 2019, 21:17
 */

#include "Ball.h"
#include <iostream>
#include <string>
#include <algorithm>
            
Ball::Ball(Map& map): 
        _currentBlockX(map.beginX()),
        _currentBlockY(map.beginY()),
        _currentBlockZ(map.beginZ()),
        _3DPosX(0.f),
        _3DPosY(0.f),
        _3DPosZ(0.f),
        _currentSide(JumperBallTypes::Direction::Up),
        _lookTowards(JumperBallTypes::Direction::North),
        _state(Ball::State::Staying),
        _stateOfLife(Ball::StateOfLife::Normal),
        _map(map),
        _mechanicsPatternJumping(),
        _mechanicsPatternFalling(0.f,0.f,0.f,0.f),
        _timeAction(std::chrono::system_clock::now()),
        _timeStateOfLife(std::chrono::system_clock::now()){
       
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
    _state = Ball::State::TurningLeft;
    setTimeActionNow();
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
    _state = Ball::State::TurningRight;
    setTimeActionNow();
}

JumperBallTypes::vec3f Ball::get3DPosition() const noexcept {
    return JumperBallTypes::vec3f {_3DPosX,_3DPosY,_3DPosZ};
}

Ball::nextBlockInformation Ball::getNextBlockInfo() const noexcept{

    struct nextBlockInformation nextBlock;

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
    auto blockAbove = _map.getBlock(aboveX,aboveY,aboveZ);
    auto blockInFrontOf = _map.getBlock(inFrontOfX,inFrontOfY,inFrontOfZ);
    auto blockLeft = _map.getBlock(leftX,leftY,leftZ);
    auto blockRight = _map.getBlock(rightX,rightY,rightZ);

    if (blockAbove && blockAbove->stillExists()) {
        nextBlock.poxX = aboveX;
        nextBlock.poxY = aboveY;
        nextBlock.poxZ = aboveZ;
        nextBlock.nextLocal = NextBlockLocal::Above;

        JumperBallTypes::Direction lookTowardsBeforeMovement = _lookTowards;

        nextBlock.nextLook = _currentSide;
        
        switch (lookTowardsBeforeMovement) {
            case JumperBallTypes::Direction::North:
                nextBlock.nextSide=JumperBallTypes::Direction::South;
                break;
            case JumperBallTypes::Direction::South:
                nextBlock.nextSide=JumperBallTypes::Direction::North;
                break;
            case JumperBallTypes::Direction::East:
                nextBlock.nextSide=JumperBallTypes::Direction::West;
                break;
            case JumperBallTypes::Direction::West:
                nextBlock.nextSide=JumperBallTypes::Direction::East;
                break;
            case JumperBallTypes::Direction::Up:
                nextBlock.nextSide=JumperBallTypes::Direction::Down;
                break;
            case JumperBallTypes::Direction::Down:
                nextBlock.nextSide=JumperBallTypes::Direction::Up;
                break;
            default :
                break;
        }

    } 
    else if (blockInFrontOf && blockInFrontOf->stillExists()) {
        
        nextBlock.poxX      = inFrontOfX;
        nextBlock.poxY      = inFrontOfY;
        nextBlock.poxZ      = inFrontOfZ;
        nextBlock.nextLocal = NextBlockLocal::InFrontOf;
        nextBlock.nextLook  = _lookTowards;
        nextBlock.nextSide  = _currentSide;
        
    } 
    else if ((!blockLeft || !blockLeft ->stillExists())
            && (!blockRight || !blockRight ->stillExists())) {
        
        nextBlock.poxX      = _currentBlockX;
        nextBlock.poxY      = _currentBlockY;
        nextBlock.poxZ      = _currentBlockZ;
        nextBlock.nextLocal = NextBlockLocal::Same;

        JumperBallTypes::Direction sideBeforeMovement = _currentSide;

        nextBlock.nextSide 	= _lookTowards;
        
        switch (sideBeforeMovement) {
            case JumperBallTypes::Direction::North:
                nextBlock.nextLook = JumperBallTypes::Direction::South;
                break;
            case JumperBallTypes::Direction::South:
                nextBlock.nextLook = JumperBallTypes::Direction::North;
                break;
            case JumperBallTypes::Direction::East:
                nextBlock.nextLook = JumperBallTypes::Direction::West;
                break;
            case JumperBallTypes::Direction::West:
                nextBlock.nextLook = JumperBallTypes::Direction::East;
                break;
            case JumperBallTypes::Direction::Up:
                nextBlock.nextLook = JumperBallTypes::Direction::Down;
                break;
            case JumperBallTypes::Direction::Down:
                nextBlock.nextLook = JumperBallTypes::Direction::Up;
                break;
            default :
                break;
        }
    }
    else {

        nextBlock.nextLocal = NextBlockLocal::None;
        nextBlock.nextSide  = _currentSide;
        nextBlock.nextLook  = _lookTowards;
        nextBlock.poxX      = _currentBlockX;
        nextBlock.poxY      = _currentBlockY;
        nextBlock.poxZ      = _currentBlockZ;
    }
            

    return nextBlock;
}


void Ball::goStraightAhead() noexcept {
    
    struct nextBlockInformation nextBlock = getNextBlockInfo(); 
    _currentBlockX  = nextBlock.poxX;
    _currentBlockY  = nextBlock.poxY;
    _currentBlockZ  = nextBlock.poxZ;
    _currentSide    = nextBlock.nextSide;
    _lookTowards    = nextBlock.nextLook; 
}

void Ball::jump() noexcept {
    _state = Ball::State::Jumping;
    setTimeActionNow();
    isGoingStraightAheadIntersectBlock();
}

void Ball::stay() noexcept {
    _state = Ball::State::Staying;
    setTimeActionNow();
}

void Ball::fall() noexcept {
    _state = Ball::State::Falling;
    setTimeActionNow();
}

void Ball::move() noexcept {
    
    struct nextBlockInformation infos = getNextBlockInfo();
    if (infos.nextLocal != NextBlockLocal::None)
    {
        _state = Ball::State::Moving;
        setTimeActionNow();
    }
}

void Ball::setTimeActionNow() noexcept {
   _timeAction = std::chrono::system_clock::now();
}

Ball::AnswerRequest Ball::doAction(Ball::ActionRequest action) {
    Ball::AnswerRequest answer (Ball::AnswerRequest::Accepted);
    switch (action) {
        case Ball::ActionRequest::GoStraightAhead:
            if (_state == Ball::State::Staying) {
                move();
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

    return answer;
}


Ball::AnswerRequest Ball::isGoingStraightAheadIntersectBlock()   noexcept {

    Ball::AnswerRequest answer = Ball::AnswerRequest::Rejected; 
    if ( _state == Ball::State::Jumping ) {
        
        answer = Ball::AnswerRequest::Accepted; 
        
        
        int aboveNearX =  _currentBlockX;
        int aboveNearY =  _currentBlockY;
        int aboveNearZ =  _currentBlockZ;
        int aboveFarX =   _currentBlockX;
        int aboveFarY =   _currentBlockY;
        int aboveFarZ =   _currentBlockZ;
        
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
        
        std::shared_ptr<const Block> blockNear  = 
                _map.getBlock(aboveNearX,aboveNearY,aboveNearZ);
        
        std::shared_ptr<const Block> blockFar   = 
                _map.getBlock(aboveFarX,aboveFarY,aboveFarZ);
        
        constexpr float distanceNear      = 1.f;
        constexpr float distanceFar       = 2.f;
        constexpr float sizeBlock         = 1.f;
        
        if (blockNear && blockNear->stillExists()) {
            _mechanicsPatternJumping.addShockFromPosition
            ( distanceNear-sizeBlock/2.f -getRadius());
        }
        else if (blockFar && blockFar->stillExists()) {
            _mechanicsPatternJumping.addShockFromPosition
            ( distanceFar-sizeBlock/2.f -getRadius());
        }
        else {
            _mechanicsPatternJumping.timesShock({});
        }
    }
    answer = Ball::AnswerRequest::Accepted; 
    return answer;
}

const ClassicalMechanics& Ball::getMechanicsJumping() const noexcept{
    return _mechanicsPatternJumping;
}

const ClassicalMechanics& Ball::getMechanicsFalling() const noexcept{
    return _mechanicsPatternFalling;
}

Ball::AnswerRequest Ball::isFallingIntersectionBlock() noexcept {

    Ball::AnswerRequest answer = Ball::AnswerRequest::Rejected; 

    if ( _state == Ball::State::Jumping  || _state == Ball::State::Falling) {
        
        float fDifference = getTimeSecondsSinceAction();

        if ( _state == Ball::State::Falling || 
                _mechanicsPatternJumping.getVelocity(fDifference).y < 0 ) {
            answer = Ball::AnswerRequest::Accepted; 
            
            auto positionBlockPtr = intersectBlock(_3DPosX,_3DPosY, _3DPosZ);
            if (positionBlockPtr) {
              _mechanicsPatternJumping.timesShock({});
              _currentBlockX      = positionBlockPtr->at(0) ;
              _currentBlockY      = positionBlockPtr->at(1) ;
              _currentBlockZ      = positionBlockPtr->at(2) ;
              _state              = Ball::State::Staying;
              _map.getBlock(_currentBlockX,_currentBlockY,_currentBlockZ)
              ->detectionEvent(_currentSide,
                           JumperBallTypesMethods::getTimePointMsFromTimePoint(
                           _timeAction));
              update();
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
            distance = 0.f;
            break;
    }

    if (distance < 0 ) distance = -distance;

    distance = offsetCenterBlock + distance ;
    if (_state== Ball::State::TurningLeft ||_state== Ball::State::TurningRight)
        distance *= 10;
    return distance;
}

std::shared_ptr<const std::vector<int> > Ball::intersectBlock(float x, 
                                                              float y, 
                                                              float z) const {
    
    std::shared_ptr<const std::vector<int> > blockIntersected = nullptr;
    
    const float offsetBlockPosition = ClassicalMechanics::radiusBall;
    float xIntersectionUnder = x;
    float yIntersectionUnder = y;
    float zIntersectionUnder = z;
    int xInteger, yInteger, zInteger;
    switch (_currentSide) {
        case JumperBallTypes::Direction::North:
            zIntersectionUnder += offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::South:
            zIntersectionUnder -= offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::East:
            xIntersectionUnder -= offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::West:
            xIntersectionUnder += offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::Up:
            yIntersectionUnder -= offsetBlockPosition ;
            break;
        case JumperBallTypes::Direction::Down:
            yIntersectionUnder += offsetBlockPosition ;
            break;
        default :
            break;
    }
    
    xInteger = static_cast<int> (xIntersectionUnder);
    yInteger = static_cast<int> (yIntersectionUnder);
    zInteger = static_cast<int> (zIntersectionUnder);
    
    auto block = _map.getBlock(xInteger,yInteger,zInteger);
    if (block && block->stillExists()) 
        blockIntersected = std::make_shared<const std::vector<int> > (
                std::initializer_list<int> ({xInteger,yInteger,zInteger}));
    
    return blockIntersected;
}

JumperBallTypes::timePointMs Ball::getTimeActionMs() const noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds>
                                                      (_timeAction);
}

JumperBallTypes::timePointMs Ball::getTimeStateOfLifeMs() const noexcept {
    return std::chrono::time_point_cast<std::chrono::milliseconds>
                                                      (_timeStateOfLife);
}

JumperBallTypes::vec3f Ball::P2DTo3D(ClassicalMechanics::physics2DVector p2D) 
                                                                           const
{

    float x = static_cast<float> (_currentBlockX + 0.5f);
    float y = static_cast<float> (_currentBlockY + 0.5f);
    float z = static_cast<float> (_currentBlockZ + 0.5f);

    const float offsetRealPosition = 0.5f + ClassicalMechanics::radiusBall;

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

    return JumperBallTypes::vec3f {x,y,z};
}

float Ball::getRadius() const {
    return ClassicalMechanics::radiusBall;
}

JumperBallTypes::Direction Ball::currentSide() const {
    return _currentSide;
}

JumperBallTypes::Direction Ball::lookTowards() const {
    return _lookTowards;
}

float Ball::getTimeActionSecondsFloat() const noexcept {

    const JumperBallTypes::timePointMs timeActionMs        = getTimeActionMs(); 
    
    const JumperBallTypes::durationMs timeActionSinceEpoch =
                                                timeActionMs.time_since_epoch();
    
    const std::chrono::duration<float> durationFloat = timeActionSinceEpoch;
    return durationFloat.count() / 1000.f ;
}

float Ball::getTimeNowSecondsFloat() noexcept {

    const JumperBallTypes::timePointMs timeNowMs = JumperBallTypesMethods::
                                                            getTimePointMSNow();
    
    const JumperBallTypes::durationMs timeNowSinceEpoch  = 
                                                  timeNowMs.time_since_epoch();
    
    const std::chrono::duration<float> durationFloat = timeNowSinceEpoch;
    
   
    return durationFloat.count() / 1000.f ;
}

float Ball::getTimeSecondsSinceAction() const noexcept{

    const JumperBallTypes::timePointMs timeNowMs = 
                                    JumperBallTypesMethods::getTimePointMSNow();
    const JumperBallTypes::durationMs timeNowSinceEpoch = 
                                                  timeNowMs.time_since_epoch();
    const JumperBallTypes::timePointMs timeActionMs = getTimeActionMs(); 
    const JumperBallTypes::durationMs timeActionSinceEpoch    = 
                                                timeActionMs.time_since_epoch();
    const JumperBallTypes::durationMs difference = 
                                      timeNowSinceEpoch - timeActionSinceEpoch;
    const std::chrono::duration<float> durationFloatDifference = difference;
    const float fDifference = durationFloatDifference.count();

    return fDifference;
}

float Ball::getTimeSecondsSinceStateOfLife() const noexcept{

    const JumperBallTypes::timePointMs timeNowMs = 
                                    JumperBallTypesMethods::getTimePointMSNow();
    const JumperBallTypes::durationMs timeNowSinceEpoch = 
                                                  timeNowMs.time_since_epoch();
    const JumperBallTypes::timePointMs timeStateOfLifeMs = 
                                                  getTimeStateOfLifeMs(); 
    const JumperBallTypes::durationMs timeStateOfLifeSinceEpoch    = 
                                          timeStateOfLifeMs.time_since_epoch();
    const JumperBallTypes::durationMs difference = 
                                  timeNowSinceEpoch - timeStateOfLifeSinceEpoch;
    const std::chrono::duration<float> durationFloatDifference = difference;
    const float fDifference = durationFloatDifference.count();

    return fDifference;
}


JumperBallTypes::vec3f Ball::lookTowardsAsVector() const {
    return JumperBallTypesMethods::directionAsVector(_lookTowards);
}

JumperBallTypes::vec3f Ball::currentSideAsVector() const {
    return JumperBallTypesMethods::directionAsVector(_currentSide);
}


JumperBallTypes::vec3f Ball::get3DPosStayingBall() const {

    constexpr float offsetPosition = 0.5f + ClassicalMechanics::radiusBall;

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

    const JumperBallTypes::vec3f position3D {x,y,z};

    return position3D;
}


void Ball::update() noexcept{


    JumperBallTypes::vec3f position3D {0.f,0.f,0.f};

    if (_state == Ball::State::Staying || _state == Ball::State::Moving ||
            _state == Ball::State::TurningLeft || 
            _state == Ball::State::TurningRight) {
        
        
        
        position3D = get3DPosStayingBall();
        if ( _state == Ball::State::Staying ){
            if (_map.getBlock(_currentBlockX,_currentBlockY,_currentBlockZ)
                    ->stillExists()) {
                _3DPosX = position3D.x;
                _3DPosY = position3D.y;
                _3DPosZ = position3D.z;
            }
            else {
                fall();
                update();
            }
            
        } else if ( _state == Ball::State::Moving){
            float sSinceAction = getTimeSecondsSinceAction();
            if (sSinceAction >= timeToGetNextBlock) {
                
                goStraightAhead();
                
                position3D  = get3DPosStayingBall();
                _3DPosX     = position3D.x;
                _3DPosY     = position3D.y;
                _3DPosZ     = position3D.z;

                _map.getBlock(_currentBlockX,_currentBlockY,_currentBlockZ)
                ->detectionEvent(_currentSide,
                           JumperBallTypesMethods::getTimePointMsFromTimePoint(
                           _timeAction));

                stay();
            }
            else {
                struct nextBlockInformation infoTarget = getNextBlockInfo();
                
                if (infoTarget.nextLocal == NextBlockLocal::InFrontOf){
                    _3DPosX = (sSinceAction * (
                            static_cast<float> (infoTarget.poxX)
                            - static_cast<float>(_currentBlockX) )
                            / timeToGetNextBlock) + position3D.x;
                    _3DPosY = (sSinceAction * (
                            static_cast<float> (infoTarget.poxY)
                            - static_cast<float>(_currentBlockY) )
                            / timeToGetNextBlock) + position3D.y;
                    _3DPosZ = (sSinceAction * (
                            static_cast<float> (infoTarget.poxZ)
                            - static_cast<float>(_currentBlockZ) )
                            / timeToGetNextBlock) + position3D.z;
                }
                else if ( infoTarget.nextLocal == NextBlockLocal::Same ||
                        infoTarget.nextLocal == NextBlockLocal::Above ) {
                    
                    float distancePerStep;
                    
                    if ( infoTarget.nextLocal == NextBlockLocal::Same)
                        distancePerStep = 0.5f + ClassicalMechanics::radiusBall;
                    
                    if ( infoTarget.nextLocal == NextBlockLocal::Above)
                        distancePerStep = 0.5f - ClassicalMechanics::radiusBall;
                    
                    float timeStep1 = sSinceAction;
                    if (timeStep1 > timeToGetNextBlock/2.f) 
                        timeStep1 = timeToGetNextBlock/2.f;
                    
                    float timeStep2 = sSinceAction - timeToGetNextBlock/2.f;
                    if (timeStep2 < 0) 
                        timeStep2 = 0;
                    
                    _3DPosX = position3D.x;
                    _3DPosY = position3D.y;
                    _3DPosZ = position3D.z;
                    
                    switch (_lookTowards) {
                        case JumperBallTypes::Direction::North:
                            _3DPosZ = static_cast<float>(position3D.z) 
                                    - distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::South:
                            _3DPosZ = static_cast<float>(position3D.z)
                                    + distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::East:
                            _3DPosX = static_cast<float>(position3D.x)
                                    + distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::West:
                            _3DPosX = static_cast<float>(position3D.x)
                                    - distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::Up:
                            _3DPosY = static_cast<float>(position3D.y)
                                    + distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::Down:
                            _3DPosY = static_cast<float>(position3D.y)
                                    - distancePerStep * timeStep1
                                    / (timeToGetNextBlock/2.f);
                            break;
                        default :
                            break;
                    }
                    switch (infoTarget.nextLook) {
                        case JumperBallTypes::Direction::North:
                            _3DPosZ = static_cast<float>(_3DPosZ) 
                                    - distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::South:
                            _3DPosZ = static_cast<float>(_3DPosZ)
                                    + distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::East:
                            _3DPosX = static_cast<float>(_3DPosX)
                                    + distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::West:
                            _3DPosX = static_cast<float>(_3DPosX)
                                    - distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::Up:
                            _3DPosY = static_cast<float>(_3DPosY)
                                    + distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        case JumperBallTypes::Direction::Down:
                            _3DPosY = static_cast<float>(_3DPosY)
                                    - distancePerStep * timeStep2
                                    / (timeToGetNextBlock/2.f);
                            break;
                        default :
                            break;
                    }
                    
                } else {
                    _3DPosX = position3D.x;
                    _3DPosY = position3D.y;
                    _3DPosZ = position3D.z;
                }
                
            }
        } else if ( _state == Ball::State::TurningLeft ||
                _state == Ball::State::TurningRight) {
            
            float sSinceAction = getTimeSecondsSinceAction();
            if (sSinceAction  >= timeToGetNextBlock) {
                stay();
                update();
            } else {
                _3DPosX = position3D.x;
                _3DPosY = position3D.y;
                _3DPosZ = position3D.z;
            }
        }
    } 
    else if ( _state == Ball::State::Jumping || 
            _state == Ball::State::Falling ) {

        ClassicalMechanics::physics2DVector pos2D;
        if (_state == Ball::State::Jumping) {
            pos2D = _mechanicsPatternJumping.
                    getPosition(getTimeSecondsSinceAction());
        } else {
            pos2D = _mechanicsPatternFalling.
                    getPosition(getTimeSecondsSinceAction());
        }

        JumperBallTypes::vec3f relativePositionJump = P2DTo3D(pos2D);
        
        position3D = {  relativePositionJump.x,
                        relativePositionJump.y,
                        relativePositionJump.z
                      };
        _3DPosX = position3D.x;
        _3DPosY = position3D.y;
        _3DPosZ = position3D.z;
        
        isFallingIntersectionBlock();
    }
    mapInteraction();
    
}

void Ball::mapInteraction() noexcept{

    Map::EffectOnBall effect = _map.interaction(_currentSide,get3DPosition());

    switch ( effect ) {
        case Map::EffectOnBall::Nothing: 
            break;
        case Map::EffectOnBall::Burnt: 
            break;
        case Map::EffectOnBall::Burst: 
            if(_stateOfLife != StateOfLife::Bursting) {
                _stateOfLife = StateOfLife::Bursting;
                _timeStateOfLife = JumperBallTypesMethods::getTimePointMSNow(); 
            }
            break;
        case Map::EffectOnBall::Slide: 
            break;
        default: break;
    }
}


Ball::State Ball::state() const {
    return _state;
}

Ball::StateOfLife Ball::stateOfLife() const {
    return _stateOfLife;
}
