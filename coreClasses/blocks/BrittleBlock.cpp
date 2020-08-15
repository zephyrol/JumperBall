/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BrittleBlock.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 octobre 2019, 12:50
 */

#include "BrittleBlock.h"

BrittleBlock::BrittleBlock():
  _stillThere(true),
  _isGoingToBreak(false),
  _collisionTime(),
  _timeUpdate(),
  _fallDirection(JBTypes::Dir::Down)
{
}


Block::categoryOfBlocksInFile BrittleBlock::getType() const {

    return Block::categoryOfBlocksInFile::Brittle;
}

void BrittleBlock::interaction( const JBTypes::Dir& ,
                                const JBTypes::timePointMs& currentTime, 
                                const JBTypes::vec3f& ,
                                const std::array<unsigned int, 3>& ) {

    constexpr float timeToFall = 1.f;
    _timeUpdate = currentTime;
    if (_isGoingToBreak && _stillThere) {
        JBTypes::durationMs diff= currentTime - _collisionTime;
        float diffF = JBTypesMethods::getFloatFromDurationMS(diff);
        if (diffF > timeToFall) {
            _stillThere = false;
        }
    }

    constexpr float fallSpeed = 20.f;
    
    if (!_stillThere)  {
        const JBTypes::vec3f dirVec =
            JBTypesMethods::directionAsVector(_fallDirection);
        const JBTypes::durationMs diff= currentTime - _collisionTime;
        const float diffF= JBTypesMethods::getFloatFromDurationMS(diff)
                            - timeToFall;
       _localTransform.at(0) = dirVec.x * diffF * fallSpeed;
       _localTransform.at(1) = dirVec.y * diffF * fallSpeed;
       _localTransform.at(2) = dirVec.z * diffF * fallSpeed;
    }

}


void BrittleBlock::setFallDirection(JBTypes::Dir ballDir) {
    switch (ballDir) {
        case JBTypes::Dir::South :
            _fallDirection = JBTypes::Dir::North;
            break;
        case JBTypes::Dir::North :
            _fallDirection = JBTypes::Dir::South;
            break;
        case JBTypes::Dir::East :
            _fallDirection = JBTypes::Dir::West;
            break;
        case JBTypes::Dir::West :
            _fallDirection = JBTypes::Dir::East;
            break;
        case JBTypes::Dir::Up :
            _fallDirection = JBTypes::Dir::Down;
            break;
        case JBTypes::Dir::Down :
            _fallDirection = JBTypes::Dir::Up;
            break;
        default :
            break;
    }
}

bool BrittleBlock::stillExists() const {
    return _stillThere;
}

void BrittleBlock::detectionEvent(const JBTypes::Dir& ballDir, 
        const JBTypes::timePointMs& currentTime) {

    if (!_isGoingToBreak) {
        _collisionTime = currentTime;
        setFallDirection(ballDir);
        _isGoingToBreak = true;
    }
}

const std::array<float, 9>& BrittleBlock::localTransform() const{

    return _localTransform;
}
