/*
 * File: BrittleBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#include "BrittleBlock.h"

BrittleBlock::BrittleBlock(const JBTypes::vec3ui &position,
                           const vecItem_sptr &items,
                           const vecEnemy_sptr &enemies,
                           const vecSpecial_sptr &specials,
                           const Ball_sptr& ball
                           ):
                           InteractiveBlock(position, items, enemies, specials, ball, true),
                           _stillThere(true),
                           _isGoingToBreak(false),
                           _collisionTime(),
                           _fallDirection(JBTypes::Dir::Down){
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

Block::Effect BrittleBlock::detectionEvent () {

    if (!_isGoingToBreak) {
        const auto ball = _ball.lock();
        _collisionTime = ball->getTimeActionMs();
        setFallDirection(ball->currentSide());
        _isGoingToBreak = true;
    }
    return Effect::Nothing;
}

bool BrittleBlock::mayDisappear() const {
    return true;
}

JBTypes::Color BrittleBlock::getColor() const {
    return JBTypes::Color::Red;
}

void BrittleBlock::update(const JBTypes::timePointMs &updatingTime) {
    InteractiveBlock::update(updatingTime);

    constexpr float timeToFall = 1.f;
    if (_isGoingToBreak && _stillThere) {
        JBTypes::durationMs diff = _updatingTime - _collisionTime;
        const float diffF = JBTypesMethods::getFloatFromDurationMS(diff);
        if (diffF > timeToFall) {
            _stillThere = false;
        }
    }

    constexpr float fallSpeed = 20.f;
    if (!_stillThere) {
        const JBTypes::vec3f dirVec = JBTypesMethods::directionAsVector(_fallDirection);
        const JBTypes::durationMs diff = _updatingTime - _collisionTime;
        const float diffF = JBTypesMethods::getFloatFromDurationMS(diff) - timeToFall;
        const float distanceTraveled = diffF * fallSpeed;
        _localTranslation.x = dirVec.x * distanceTraveled;
        _localTranslation.y = dirVec.y * distanceTraveled;
        _localTranslation.z = dirVec.z * distanceTraveled;
    }
}

SceneElement::GlobalState BrittleBlock::getGlobalState() const {
    return _stillThere ? SceneElement::GlobalState::United: SceneElement::GlobalState::Separate;
}

bool BrittleBlock::globalStateMayChange() const {
    return true;
}

