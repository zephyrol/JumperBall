//
// DarkBall.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "DarkBall.h"

DarkBall::DarkBall(
    const JBTypes::vec3ui &initialPosition,
    const JBTypes::Dir &dir,
    const JBTypes::Dir &movementDirection,
    size_t nbOfJumps,
    const Ball_sptr &ball
) :
    InteractiveEnemy(
        initialPosition,
        dir,
        darkBallRadius,
        nbOfJumps,
        ball),
    _movementDirection(movementDirection) {
}

void DarkBall::update() {
    const ClassicalMechanics &mechanics = darkBallClassicalMechanics;

    const JBTypes::vec3f vecDirMovement = JBTypesMethods::directionAsVector(_movementDirection);

    const JBTypes::vec3f vecDirSide = JBTypesMethods::directionAsVector(direction());

    const auto nbOfJumpsDone = static_cast <size_t>(timeSinceCreation / mechanics.getTimeToGetDestination());

    const float distanceAlreadyTravelled = nbOfJumpsDone * mechanics.getJumpDistance();

    const std::function<float(float, float)> getRemainder =
        [](float dividend, float divisor)
            -> float {
            const auto quotient = static_cast <size_t>(dividend / divisor);
            return dividend - (divisor * static_cast <float>(quotient));
        };

    const float timeCurrentJump = getRemainder(
        timeSinceCreation,
        mechanics.getTimeToGetDestination()
    );

    const ClassicalMechanics::physics2DVector positionCurrentJump = mechanics.getPosition(timeCurrentJump);
    const float sidePosition = positionCurrentJump.y;

    const float distanceTravelled = positionCurrentJump.x + distanceAlreadyTravelled;

    std::function<float(float, float, size_t)> getMovementPosition =
        [&getRemainder](float distanceTravelled,
                        float jumpDistance,
                        size_t length) -> float {
            const unsigned int nbOfjumps =
                static_cast <size_t>(distanceTravelled / jumpDistance);
            const float reminder = getRemainder(distanceTravelled, jumpDistance);

            return ((nbOfjumps / length) % 2) == 0
                   ? (nbOfjumps % length) * jumpDistance + reminder
                   : length * jumpDistance -
                     ((nbOfjumps % length) * jumpDistance + reminder);
        };

    const float movementPosition = getMovementPosition(
        distanceTravelled,
        mechanics.getJumpDistance(),
        length()
    );

    _position.x = _initialPosition.x + vecDirMovement.x * movementPosition + vecDirSide.x * sidePosition;
    _position.y = _initialPosition.y + vecDirMovement.y * movementPosition + vecDirSide.y * sidePosition;
    _position.z = _initialPosition.z + vecDirMovement.z * movementPosition + vecDirSide.z * sidePosition;
}

const JBTypes::Dir &DarkBall::movementDirection() const {
    return _movementDirection;
}

bool DarkBall::touchingTest() const {

    const auto ball = _ball.lock();
    const auto &boundingSpherePosition = ball->get3DPosition();
    const auto &boundingSphereRadius = ball->getRadius();

    return JBTypesMethods::distance(boundingSpherePosition, _position) <
           (boundingSphereRadius + darkBallRadius);
}

const ClassicalMechanics DarkBall::darkBallClassicalMechanics(DarkBall::darkBallRadius);

vecCstShape_sptr DarkBall::getShapes() const {
    const auto &diameter = this->size();
    const auto darkBallShape = std::make_shared<const Shape>(
        Shape::Aspect::Sphere,
        JBTypes::Color::Blue,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale, {diameter, diameter, diameter})
            }
        )
    );
    return {darkBallShape};
}
