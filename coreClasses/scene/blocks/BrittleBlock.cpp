/*
 * File: BrittleBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#include "BrittleBlock.h"

BrittleBlock::BrittleBlock(const JBTypes::vec3ui& position):
    Block(position, true, false),
    _stillThere(true),
    _isGoingToBreak(false),
    _collisionTime(),
    _timeUpdate(),
    _fallDirection(JBTypes::Dir::Down) {
}

Block::Effect BrittleBlock::interaction (const JBTypes::Dir&,
                                         const JBTypes::timePointMs& currentTime,
                                         const JBTypes::vec3f&
                                         ) {
    constexpr float timeToFall = 1.f;
    _timeUpdate = currentTime;
    if (_isGoingToBreak && _stillThere) {
        JBTypes::durationMs diff = currentTime - _collisionTime;
        const float diffF = JBTypesMethods::getFloatFromDurationMS(diff);
        if (diffF > timeToFall) {
            _stillThere = false;
        }
    }

    constexpr float fallSpeed = 20.f;
    if (!_stillThere) {
        const JBTypes::vec3f dirVec = JBTypesMethods::directionAsVector(_fallDirection);
        const JBTypes::durationMs diff = currentTime - _collisionTime;
        const float diffF = JBTypesMethods::getFloatFromDurationMS(diff) - timeToFall;
        const float distanceTraveled = diffF * fallSpeed;
        _localTranslation.x = dirVec.x * distanceTraveled;
        _localTranslation.y = dirVec.y * distanceTraveled;
        _localTranslation.z = dirVec.z * distanceTraveled;

    }
    return Block::Effect::Nothing;
}


void BrittleBlock::setFallDirection (JBTypes::Dir direction) {
    switch (direction) {
    case JBTypes::Dir::South:
        _fallDirection = JBTypes::Dir::North;
        break;
    case JBTypes::Dir::North:
        _fallDirection = JBTypes::Dir::South;
        break;
    case JBTypes::Dir::East:
        _fallDirection = JBTypes::Dir::West;
        break;
    case JBTypes::Dir::West:
        _fallDirection = JBTypes::Dir::East;
        break;
    case JBTypes::Dir::Up:
        _fallDirection = JBTypes::Dir::Down;
        break;
    case JBTypes::Dir::Down:
        _fallDirection = JBTypes::Dir::Up;
        break;
    default:
        break;
    }
}

bool BrittleBlock::isExists() const {
    return _stillThere;
}

Block::Effect BrittleBlock::detectionEvent (const JBTypes::Dir& direction,
                                            const JBTypes::timePointMs& currentTime) {

    if (!_isGoingToBreak) {
        _collisionTime = currentTime;
        setFallDirection(direction);
        _isGoingToBreak = true;
    }
    return Effect::Nothing;
}
