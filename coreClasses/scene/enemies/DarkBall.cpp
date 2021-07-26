//
// DarkBall.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "DarkBall.h"

DarkBall::DarkBall(const JBTypes::vec3ui& initialPosition,
                   const JBTypes::Dir& dir,
                   const JBTypes::Dir& movementDirection,
                   size_t nbOfJumps):
    Enemy(initialPosition,
          dir,
          darkBallRadius,
          nbOfJumps,
          { 0.f,
            0.f,
            0.f,
            0.f,
            0.f,
            0.f,
            1.f,
            1.f,
            1.f }),
    _movementDirection(movementDirection) {
}

Enemy::Effect DarkBall::update (const JBTypes::vec3f& boundingSpherePosition,
                                float boundingSphereRadius) {
    const ClassicalMechanics& mechanics = darkBallClassicalMechanics;
    const float timeSinceCreation =
        JBTypesMethods::getTimeSecondsSinceTimePoint(creationTime());

    const JBTypes::vec3f vecDirMovement =
        JBTypesMethods::directionAsVector(_movementDirection);

    const JBTypes::vec3f vecDirSide =
        JBTypesMethods::directionAsVector(direction());

    const size_t nbOfJumpsDone = static_cast <size_t>(timeSinceCreation /
                                                      mechanics.getTimeToGetDestination());

    const float distanceAlreadyTravelled = nbOfJumpsDone *
                                           mechanics.getJumpDistance();

    const std::function <float(float, float)> getRemainder =
        [] (float dividend, float divisor)
        ->float {
            const size_t quotient = static_cast <size_t>(dividend / divisor);
            return dividend - (divisor * static_cast <float>(quotient));
        };

    const float timeCurrentJump = getRemainder(
        timeSinceCreation,
        mechanics.getTimeToGetDestination()
        );

    const ClassicalMechanics::physics2DVector positionCurrentJump =
        mechanics.getPosition(timeCurrentJump);
    const float sidePosition = positionCurrentJump.y;

    const float distanceTravelled =
        positionCurrentJump.x + distanceAlreadyTravelled;

    std::function <float(float, float, size_t)> getMovementPosition =
        [&getRemainder] (float distanceTravelled,
                         float jumpDistance,
                         size_t length)->float {
            const unsigned int nbOfjumps =
                static_cast <size_t>(distanceTravelled / jumpDistance);
            const float reminder = getRemainder(distanceTravelled, jumpDistance);

            return ((nbOfjumps / length) % 2) == 0
                   ? (nbOfjumps % length) * jumpDistance + reminder
                   : length*jumpDistance -
                   ((nbOfjumps % length) * jumpDistance + reminder);
        };

    const float movementPosition = getMovementPosition(
        distanceTravelled,
        mechanics.getJumpDistance(),
        length()
        );
    _transform.at(0) =
        vecDirMovement.x * movementPosition + vecDirSide.x * sidePosition;
    _transform.at(1) =
        vecDirMovement.y * movementPosition + vecDirSide.y * sidePosition;
    _transform.at(2) =
        vecDirMovement.z * movementPosition + vecDirSide.z * sidePosition;


    _position.x = _initialPosition.x + _transform.at(0);
    _position.y = _initialPosition.y + _transform.at(1);
    _position.z = _initialPosition.z + _transform.at(2);

    touchingTest(boundingSpherePosition, boundingSphereRadius);
    return _hasHit
           ? Enemy::Effect::Burst
           : Enemy::Effect::Nothing;
}

const JBTypes::Dir& DarkBall::movementDirection() const {
    return _movementDirection;
}

void DarkBall::touchingTest (const JBTypes::vec3f& boundingSpherePosition,
                             float boundingSphereRadius) {
    if (
        JBTypesMethods::distance(boundingSpherePosition, _position) <
        (boundingSphereRadius + darkBallRadius)
        ) {
        _intersectionTime = JBTypesMethods::getTimePointMSNow();
        _hasHit = true;
    }
}

const ClassicalMechanics DarkBall::darkBallClassicalMechanics(
    DarkBall::darkBallRadius);
