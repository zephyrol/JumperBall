//
// ThornBall.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "ThornBall.h"

ThornBall::ThornBall(
    const JBTypes::vec3ui &initialPosition,
    const JBTypes::Dir &dir,
    const JBTypes::Dir &movementDirection,
    size_t movementLength,
    const Ball_sptr &ball
) :
    InteractiveEnemy(
        initialPosition,
        dir,
        thornBallRadius,
        movementLength,
        ball
    ),
    _chronometer(ball->getInGameChronometer()),
    _movementDirection(movementDirection) {
}

void ThornBall::update() {
    constexpr float movementDuration = 2.f;

    const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(_movementDirection);

    const auto movementLength = static_cast <float>(length());
    const float localMovement = (1.f - cosf(2.f * static_cast <float>(M_PI) *
                                            //TODO replace by in game time
                                            (_chronometer->getTime() / movementDuration))
                                ) / 2.f;
    const float movementPosition = localMovement * movementLength;

    _position.x = _initialPosition.x + vecDir.x * movementPosition;
    _position.y = _initialPosition.y + vecDir.y * movementPosition;
    _position.z = _initialPosition.z + vecDir.z * movementPosition;
}

const JBTypes::Dir &ThornBall::movementDirection() const {
    return _movementDirection;
}

bool ThornBall::touchingTest() const {
    const auto ball = _ball.lock();
    const auto &boundingSpherePosition = ball->get3DPosition();
    const auto &boundingSphereRadius = ball->getRadius();
    return JBTypesMethods::distance(boundingSpherePosition, _position) <
           (boundingSphereRadius + thornBallRadius);
}

vecCstShape_sptr ThornBall::getShapes() const {
    const auto &diameter = this->size();
    const auto thornBallShape = std::make_shared<const Shape>(
        Shape::Aspect::Sphere,
        JBTypes::Color::Red,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {diameter, diameter, diameter})
            }
        )
    );
    return {thornBallShape};
}

std::string ThornBall::getDynamicGroupHash() const {
    return "thornBall;" + std::to_string(_initialPosition.x) + "," + std::to_string(_initialPosition.y) + ","
           + std::to_string(_initialPosition.z) + "," + std::to_string(static_cast<int>(_movementDirection));
}
