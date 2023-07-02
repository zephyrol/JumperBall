/*
 * File: BrittleBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#include "BrittleBlock.h"
#include "system/SoundOutput.h"

BrittleBlock::BrittleBlock(
    const JBTypes::vec3ui &position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball
) :
    InteractiveBlock(position, items, enemies, specials, ball, true),
    _chronometer(ball->getInGameChronometer()),
    _stillThere(true),
    _isGoingToBreak(false),
    _collisionTime(0.f),
    _fallDirection(JBTypes::Dir::Down) {
}

void BrittleBlock::setFallDirection(JBTypes::Dir direction) {
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

Block::Effect BrittleBlock::detectionEvent() {

    if (!_isGoingToBreak) {
        const auto ball = _ball.lock();
        _collisionTime = ball->getActionTime();
        setFallDirection(ball->currentSide());
        _isGoingToBreak = true;

        const auto lookTowardsVec = ball->lookTowardsAsVector();
        _localRotation.x = [&lookTowardsVec]() {
            if (!JBTypesMethods::floatsEqual(lookTowardsVec.x, 0.f)) {
                return 1.f;
            }
            if (!JBTypesMethods::floatsEqual(lookTowardsVec.y, 0.f)) {
                return 2.f;
            }
            return 3.f;
        }();

        ball->addUpdateOutput(std::make_shared<SoundOutput>("blockIsGoingToFall"));
    }
    return Effect::Nothing;
}

bool BrittleBlock::mayDisappear() const {
    return true;
}

JBTypes::Color BrittleBlock::getColor() const {
    return JBTypes::Color::Red;
}

void BrittleBlock::update() {
    InteractiveBlock::update();

    constexpr float timeToFall = 1.f;
    if (_isGoingToBreak && _stillThere) {
        // TODO Use in game time
        const auto diff = _chronometer->getTime() - _collisionTime;
        constexpr auto shakingTime = 0.15f; // in seconds
        const auto shakingPeriod = sinf(diff * 2.f * static_cast<float>(M_PI) / shakingTime);
        constexpr auto maxAngle = 0.08f; // in radians
        if (diff > timeToFall) {
            _localRotation = {0.f, 0.f};
            _stillThere = false;
            _ball.lock()->addUpdateOutput(std::make_shared<SoundOutput>("blockIsFalling"));
        } else {
            _localRotation.y = shakingPeriod * maxAngle;
        }
    }

    constexpr float fallSpeed = 20.f;
    if (!_stillThere) {
        const JBTypes::vec3f dirVec = JBTypesMethods::directionAsVector(_fallDirection);
        // TODO Use in game time
        const auto diff = _chronometer->getTime() - _collisionTime;
        const float diffTimeToFall = diff - timeToFall;
        const float distanceTraveled = diffTimeToFall * fallSpeed;
        _localTranslation.x = dirVec.x * distanceTraveled;
        _localTranslation.y = dirVec.y * distanceTraveled;
        _localTranslation.z = dirVec.z * distanceTraveled;
    }
}

std::string BrittleBlock::getDynamicGroupHash() const {
    return "brittle;" + std::to_string(position().at(0)) + "," +
           std::to_string(position().at(1)) + "," + std::to_string(position().at(2));
}

