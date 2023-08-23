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

Displayable::DynamicValues<float> Item::getDynamicFloatValues() const {
    return {0.f};
}

Displayable::DynamicValues<JBTypes::vec3f> Item::getDynamicVec3fValues() const {

    constexpr auto thresholdSecondStep = 1.f;

    const auto computeScale = [this]() -> JBTypes::vec3f {
        if (_obtainingTime == nullptr) {
            return {1.f, 1.f, 1.f};
        }
        const auto obtainingTime = *_obtainingTime;
        if (obtainingTime < thresholdSecondStep) {
            return {1.f, 1.f, 1.f};
        }
        constexpr auto thresholdThirdStep = 1.5f;
        constexpr auto durationSecondStep = thresholdThirdStep - thresholdSecondStep;
        if (obtainingTime < thresholdThirdStep) {
            const auto scale = 1.f + ((obtainingTime - thresholdSecondStep) / durationSecondStep);
            return {scale, scale, scale};
        }

        constexpr auto durationThirdStep = 0.2f;
        if (obtainingTime < thresholdThirdStep + durationThirdStep) {
            const auto scale = 2.f * (1.f - ((obtainingTime - thresholdThirdStep) / durationThirdStep));
            return {scale, scale, scale};
        }
        return {0.f, 0.f, 0.f};
    };
    const auto computeLocalTranslation = [this]() -> JBTypes::vec3f {
        if (_obtainingTime == nullptr) {
            return {0.f, 0.f, 0.f};
        }
        const auto obtainingTime = *_obtainingTime;
        if (obtainingTime < thresholdSecondStep) {
            float translateCoeff = obtainingTime / thresholdSecondStep;
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
