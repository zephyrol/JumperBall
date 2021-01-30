//
// Laser.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "Laser.h"

Laser::Laser(const JBTypes::Color& color,
             const Block& tieBlock,
             const JBTypes::vec3ui& initialPosition,
             const JBTypes::Dir& dir,
             size_t length):
    Enemy(tieBlock,
          initialPosition,
          dir,
          laserRadius,
          length,
          { 0.f,
            0.f,
            0.f,
            0.f,
            0.f,
            0.f,
            1.f,
            1.f,
            1.f }),
    _color(color),
    _isActivated(true) {
}

Enemy::Effect Laser::update (const JBTypes::vec3f& entityPosition,
                             float radiusEntity) {
    touchingTest(entityPosition, radiusEntity);

    if (_isActivated) {
        _transform.at(6) = 1.f;
        _transform.at(7) = 1.f;
        _transform.at(8) = 1.f;
    } else {
        _transform.at(6) = 0.f;
        _transform.at(7) = 0.f;
        _transform.at(8) = 0.f;
    }
    return _hasHit
           ? Enemy::Effect::Burst
           : Enemy::Effect::Nothing;
}

JBTypes::Color Laser::getColor() const {
    return _color;
}

void Laser::switchOnOff() {
    _isActivated = !_isActivated;
}

void Laser::touchingTest (const JBTypes::vec3f& entityPosition,
                          float radiusEntity) {
    if (!_isActivated) {
        return;
    }
    constexpr float offsetLaser = 0.5f;

    const float entityMinX = entityPosition.x - radiusEntity;
    const float entityMaxX = entityPosition.x + radiusEntity;
    const float entityMinY = entityPosition.y - radiusEntity;
    const float entityMaxY = entityPosition.y + radiusEntity;
    const float entityMinZ = entityPosition.z - radiusEntity;
    const float entityMaxZ = entityPosition.z + radiusEntity;

    const float fLength = static_cast <float>(length());
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

    if (
        (entityMinX < laserMaxX && entityMaxX > laserMinX) &&
        (entityMinY < laserMaxY && entityMaxY > laserMinY) &&
        (entityMinZ < laserMaxZ && entityMaxZ > laserMinZ)
        ) {
        _intersectionTime = JBTypesMethods::getTimePointMSNow();
        _hasHit = true;
    }
}
