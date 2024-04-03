/*
 * File: Item.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#include "Item.h"

Item::Item(const JBTypes::vec3ui &position, const JBTypes::Dir &direction, CstChronometer_sptr chronometer) :
    _chronometer(std::move(chronometer)),
    _position(position),
    _direction(direction),
    _directionVec(JBTypesMethods::directionAsVector(_direction)),
    _translationWay(JBTypesMethods::scalarApplication(0.9f, _directionVec)),
    _3DPosition(compute3DPosition()),
    _obtainingTime(nullptr) {
}

const JBTypes::vec3ui &Item::position() const {
    return _position;
}

const JBTypes::vec3f &Item::get3DPosition() const {
    return _3DPosition;
}

bool Item::isGotten() const {
    return _obtainingTime != nullptr;
}

float Item::getTimeSinceObtaining() const {
    return _obtainingTime != nullptr
           ? _chronometer->getTime() - *_obtainingTime
           : 0;
}

JBTypes::vec3f Item::compute3DPosition() const {
    constexpr float offsetPosition = 0.755f;
    auto x = static_cast <float>(_position.at(0));
    auto y = static_cast <float>(_position.at(1));
    auto z = static_cast <float>(_position.at(2));

    switch (_direction) {
        case JBTypes::Dir::North:
            z -= offsetPosition;
            break;
        case JBTypes::Dir::South:
            z += offsetPosition;
            break;
        case JBTypes::Dir::East:
            x += offsetPosition;
            break;
        case JBTypes::Dir::West:
            x -= offsetPosition;
            break;
        case JBTypes::Dir::Up:
            y += offsetPosition;
            break;
        case JBTypes::Dir::Down:
            y -= offsetPosition;
            break;
        default:
            break;
    }

    return JBTypes::vec3f{x, y, z};
}

const JBTypes::Dir &Item::direction() const {
    return _direction;
}

void Item::setAsGotten() {
    _obtainingTime = std::unique_ptr<float>(new float(_chronometer->getTime()));
}

std::string Item::getDynamicGroupHash() const {
    return "item;" + std::to_string(_position.at(0)) + "," +
           std::to_string(_position.at(1)) + "," +
           std::to_string(_position.at(2)) + ";" +
           std::to_string(static_cast<int>(_direction));
}

Transformation Item::getVerticalCylinderRotation(const JBTypes::Dir &direction) {
    const auto rotationVector = [](const JBTypes::Dir &direction) -> JBTypes::vec3f {

        switch (direction) {
            case JBTypes::Dir::North:
                return {0.f, 0.f, 0.f};
            case JBTypes::Dir::South:
                return {JBTypes::pi, 0.f, 0.f};
            case JBTypes::Dir::East:
                return {JBTypes::pi2, JBTypes::pi2, 0.f};
            case JBTypes::Dir::West:
                return {-JBTypes::pi2, -JBTypes::pi2, 0.f};
            case JBTypes::Dir::Up:
                return {JBTypes::pi2, 0.f, 0.f};
            case JBTypes::Dir::Down:
                return {-JBTypes::pi2, 0.f, 0.f};
            default:
                return {0.0f, 0.0f, 0.0f};
        }
    }(direction);
    return Transformation(Transformation::Type::Rotation, rotationVector);
}

Displayable::DynamicValues<JBTypes::vec3f> Item::getDynamicVec3fValues() const {


    const auto timeSinceObtaining = getTimeSinceObtaining();
    const auto computeScale = [&timeSinceObtaining]() -> JBTypes::vec3f {
        constexpr auto thresholdSecondStep = 1.f;
        if (timeSinceObtaining < thresholdSecondStep) {
            return {1.f, 1.f, 1.f};
        }
        constexpr auto thresholdThirdStep = 1.5f;
        constexpr auto durationSecondStep = thresholdThirdStep - thresholdSecondStep;
        if (timeSinceObtaining < thresholdThirdStep) {
            const auto scale = 1.f + ((timeSinceObtaining - thresholdSecondStep) / durationSecondStep);
            return {scale, scale, scale};
        }

        constexpr auto durationThirdStep = 0.2f;
        if (timeSinceObtaining < thresholdThirdStep + durationThirdStep) {
            const auto scale = 2.f * (1.f - ((timeSinceObtaining - thresholdThirdStep) / durationThirdStep));
            return {scale, scale, scale};
        }
        return {0.f, 0.f, 0.f};
    };
    const auto computeLocalTranslation = [this, &timeSinceObtaining]() -> JBTypes::vec3f {
        constexpr auto thresholdSecondStep = 1.f;
        if (timeSinceObtaining < thresholdSecondStep) {
            float translateCoeff = timeSinceObtaining / thresholdSecondStep;
            return JBTypesMethods::scalarApplication(translateCoeff, _translationWay);
        }
        return _translationWay;
    };

    return {JBTypesMethods::add(_3DPosition, computeLocalTranslation()), computeScale()};
}

Displayable::DynamicValues<JBTypes::Quaternion> Item::getDynamicQuaternionValues() const {
    if (_obtainingTime != nullptr) {
        constexpr auto speedPow = 5.f;
        const auto angle = powf(getTimeSinceObtaining(), speedPow);
        return {{JBTypesMethods::createRotationQuaternion(_directionVec, angle)}};
    }
    constexpr auto speedFactor = 5.f;
    return {{JBTypesMethods::createRotationQuaternion(_directionVec, speedFactor * _chronometer->getTime())}};
}
