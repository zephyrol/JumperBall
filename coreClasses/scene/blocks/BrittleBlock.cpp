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
    _fPosition{
        static_cast<float>(position.at(0)),
        static_cast<float>(position.at(1)),
        static_cast<float>(position.at(2))
    },
    _stillThere(true),
    _isGoingToBreak(false),
    _collisionTime(0.f),
    _fallingDirection{0.f, -1.f, 0.0},
    _fallingRotationAxis{1.f, 0.f, 0.0},
    _shakingRotation(JBTypesMethods::createQuaternion({0.f, 0.f, 0.f}, 1.f)),
    _localTranslation{0.f, 0.f, 0.f} {
}

void BrittleBlock::setFallingDirection(JBTypes::Dir direction) {
    JBTypes::Dir fallDirection;
    switch (direction) {
        case JBTypes::Dir::South:
            fallDirection = JBTypes::Dir::North;
            break;
        case JBTypes::Dir::North:
            fallDirection = JBTypes::Dir::South;
            break;
        case JBTypes::Dir::East:
            fallDirection = JBTypes::Dir::West;
            break;
        case JBTypes::Dir::West:
            fallDirection = JBTypes::Dir::East;
            break;
        case JBTypes::Dir::Up:
            fallDirection = JBTypes::Dir::Down;
            break;
        case JBTypes::Dir::Down:
            fallDirection = JBTypes::Dir::Up;
            break;
        default:
            break;
    }
    _fallingDirection = JBTypesMethods::directionAsVector(fallDirection);
}

bool BrittleBlock::isExists() const {
    return _stillThere;
}

Block::Effect BrittleBlock::detectionEvent() {
    InteractiveBlock::detectionEvent();
    if (!_isGoingToBreak) {
        const auto ball = _ball.lock();
        _collisionTime = ball->getActionTime();
        setFallingDirection(ball->currentSide());
        _fallingRotationAxis = ball->lookTowardsAsVector();
        _isGoingToBreak = true;
        ball->addUpdateOutput(std::make_shared<SoundOutput>("blockIsGoingToFall"));
    }
    return Effect::Nothing;
}

bool BrittleBlock::mayDisappear() const {
    return true;
}

JBTypes::Color BrittleBlock::getColor() const {
    return JBTypes::Color::White;
}

void BrittleBlock::update() {
    InteractiveBlock::update();

    constexpr float timeToFall = 0.6f;
    if (_isGoingToBreak && _stillThere) {
        const auto diff = _chronometer->getTime() - _collisionTime;
        if (diff > timeToFall) {
            _shakingRotation = {{0.f, 0.f, 0.f}, 1.f};
            _stillThere = false;
            _ball.lock()->addUpdateOutput(std::make_shared<SoundOutput>("blockIsFalling"));
        } else {
            constexpr auto shakingTime = 0.15f; // in seconds
            const auto shakingPeriod = sinf(diff * 2.f * JBTypes::pi / shakingTime);
            constexpr auto maxAngle = 0.08f; // in radians
            const auto angle = shakingPeriod * maxAngle;
            _shakingRotation = JBTypesMethods::createRotationQuaternion(_fallingRotationAxis, angle);
        }
    }

    constexpr float fallSpeed = 20.f;
    if (!_stillThere) {
        const auto diff = _chronometer->getTime() - _collisionTime;
        const float diffTimeToFall = diff - timeToFall;
        const float distanceTraveled = diffTimeToFall * fallSpeed;
        _localTranslation = JBTypesMethods::scalarApplication(distanceTraveled, _fallingDirection);
    }
}

std::string BrittleBlock::getDynamicGroupHash() const {
    return "brittleBlock;" + std::to_string(position().at(0)) + "," +
           std::to_string(position().at(1)) + "," + std::to_string(position().at(2));
}

Displayable::DynamicValues<JBTypes::vec3f> BrittleBlock::getDynamicVec3fValues() const {
    return {JBTypesMethods::add(_localTranslation, _fPosition), {1.f, 1.f, 1.f}};
}

Displayable::DynamicValues <JBTypes::Quaternion> BrittleBlock::getDynamicQuaternionValues() const {
    return{{_shakingRotation}};
}

