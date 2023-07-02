//
// Laser.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "Laser.h"

Laser::Laser(
    const JBTypes::Color &color,
    const JBTypes::vec3ui &initialPosition,
    const JBTypes::Dir &dir,
    size_t length,
    const Ball_sptr &ball,
    bool isActivated
) :
    InteractiveEnemy(
        initialPosition,
        dir,
        laserRadius,
        length,
        ball,
        isActivated
    ),
    _color(JBTypesMethods::colorToShiny(color)) {
}

JBTypes::Color Laser::getColor() const {
    return _color;
}

void Laser::switchOnOff() {
    _isActivated = !_isActivated;
    _scale = _isActivated ? Enemy::scaleActivated : Enemy::scaleDisable;
}

bool Laser::touchingTest() const {
    if (!_isActivated) {
        return false;
    }

    const auto ball = _ball.lock();
    const auto &boundingSpherePosition = ball->get3DPosition();
    const auto &boundingSphereRadius = ball->getRadius();

    constexpr float offsetLaser = 0.5f;

    const float entityMinX = boundingSpherePosition.x - boundingSphereRadius;
    const float entityMaxX = boundingSpherePosition.x + boundingSphereRadius;
    const float entityMinY = boundingSpherePosition.y - boundingSphereRadius;
    const float entityMaxY = boundingSpherePosition.y + boundingSphereRadius;
    const float entityMinZ = boundingSpherePosition.z - boundingSphereRadius;
    const float entityMaxZ = boundingSpherePosition.z + boundingSphereRadius;

    const auto fLength = static_cast <float>(length());
    const float laserMinX = direction() == JBTypes::Dir::West
                            ? _position.x - offsetLaser - fLength
                            : _position.x - offsetLaser;
    const float laserMaxX = direction() == JBTypes::Dir::East
                            ? _position.x + offsetLaser + fLength
                            : _position.x + offsetLaser;
    const float laserMinY = direction() == JBTypes::Dir::Down
                            ? _position.y - offsetLaser - fLength
                            : _position.y - offsetLaser;
    const float laserMaxY = direction() == JBTypes::Dir::Up
                            ? _position.y + offsetLaser + fLength
                            : _position.y + offsetLaser;
    const float laserMinZ = direction() == JBTypes::Dir::South
                            ? _position.z - offsetLaser - fLength
                            : _position.z - offsetLaser;
    const float laserMaxZ = direction() == JBTypes::Dir::North
                            ? _position.z + offsetLaser + fLength
                            : _position.z + offsetLaser;

    return (entityMinX < laserMaxX && entityMaxX > laserMinX) &&
           (entityMinY < laserMaxY && entityMaxY > laserMinY) &&
           (entityMinZ < laserMaxZ && entityMaxZ > laserMinZ);
}

vecCstShape_sptr Laser::getShapes() const {

    vecCstShape_sptr shapes{};

    constexpr float offsetLaserSide = 0.15f;

    const std::array<JBTypes::vec3f, 4> translationsFloor{
        JBTypes::vec3f{-offsetLaserSide, 0.f, -offsetLaserSide},
        JBTypes::vec3f{offsetLaserSide, 0.f, -offsetLaserSide},
        JBTypes::vec3f{-offsetLaserSide, 0.f, offsetLaserSide},
        JBTypes::vec3f{offsetLaserSide, 0.f, offsetLaserSide}
    };

    const JBTypes::vec3f scale{size(), static_cast <float>(length()), size()};
    const auto directionRotation = JBTypesMethods::rotationVectorUpToDir(direction());

    for (const auto &translationFloor: translationsFloor) {

        shapes.push_back(std::make_shared<const Shape>(
            Shape::Aspect::Cylinder,
            getColor(),
            std::initializer_list<Transformation>(
                {
                    Transformation(Transformation::Type::Scale, scale),
                    Transformation(Transformation::Type::Translation,
                                   translationFloor),
                    Transformation(Transformation::Type::Rotation,
                                   directionRotation)
                }
            )
        ));
    }
    return shapes;
}

std::string Laser::getDynamicGroupHash() const {
    return "laser;" + std::to_string(static_cast<int>(_color));
}

bool Laser::dynamicsMayChange() const {
    return true;
}
