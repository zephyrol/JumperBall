//
// ThornBall.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "ThornBall.h"

ThornBall::ThornBall(const JBTypes::vec3ui& initialPosition,
                     const JBTypes::Dir& dir,
                     const JBTypes::Dir& movementDirection,
                     size_t movementLength,
                     const Ball_sptr &ball
                     ):
    Enemy(initialPosition,
          dir,
          thornBallRadius,
          movementLength,
          { 0.f,
            0.f,
            0.f,
            0.f,
            0.f,
            0.f,
            1.f,
            1.f,
            1.f },
          ball),
    _movementDirection(movementDirection) {
}

Enemy::Effect ThornBall::update (const JBTypes::vec3f& boundingSpherePosition,
                                 float boundingSphereRadius) {
    constexpr float movementDuration = 2.f;
    const float timeSinceCreation =
        JBTypesMethods::getTimeSecondsSinceTimePoint(creationTime());

    const JBTypes::vec3f vecDir =
        JBTypesMethods::directionAsVector(_movementDirection);

    const auto movementLength = static_cast <float>(length());
    const float localMovement = (1.f - cosf(2.f * static_cast <float>(M_PI) *
                                            (timeSinceCreation / movementDuration))) / 2.f;
    const float movementPosition = localMovement * movementLength;

    _transform.at(0) = vecDir.x * movementPosition;
    _transform.at(1) = vecDir.y * movementPosition;
    _transform.at(2) = vecDir.z * movementPosition;

    _position.x = _initialPosition.x + _transform.at(0);
    _position.y = _initialPosition.y + _transform.at(1);
    _position.z = _initialPosition.z + _transform.at(2);
    touchingTest(boundingSpherePosition, boundingSphereRadius);
    return _hasHit
           ? Enemy::Effect::Burst
           : Enemy::Effect::Nothing;
}

const JBTypes::Dir& ThornBall::movementDirection() const {
    return _movementDirection;
}

void ThornBall::touchingTest (const JBTypes::vec3f& boundingSpherePosition,
                              float boundingSphereRadius) {
    if (
        JBTypesMethods::distance(boundingSpherePosition, _position) <
        (boundingSphereRadius + thornBallRadius)
        ) {
        _intersectionTime = JBTypesMethods::getTimePointMSNow();
        _hasHit = true;
    }
}
