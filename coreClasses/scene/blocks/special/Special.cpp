/*
 * File: Special.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "Special.h"

Special::Special(
    CstChronometer_sptr chronometer,
    const JBTypes::Color &color,
    const JBTypes::Dir &dir,
    const glm::u32vec3 &position,
    bool isActivated
) :
    _chronometer(std::move(chronometer)),
    _direction(dir),
    _directionVec(JBTypesMethods::directionAsVector(_direction)),
    _color(color),
    _colorAttributeName("is" + JBTypesMethods::colorToString(_color) + "Activated"),
    _position(position),
    _positionF([this]() {
        const glm::vec3 positionF{
            static_cast<float>(_position[0]),
            static_cast<float>(_position[1]),
            static_cast<float>(_position[2])
        };
        return JBTypesMethods::add(positionF, JBTypesMethods::scalarApplication(0.5f, _directionVec));
    }()),
    _isActivated(isActivated) {
}

const JBTypes::Color &Special::getColor() const {
    return _color;
}

const JBTypes::Dir &Special::direction() const {
    return _direction;
}

const glm::vec3 &Special::positionF() const {
    return _positionF;
}

const glm::u32vec3 &Special::position() const {
    return _position;
}

bool Special::isActivated() const {
    return _isActivated;
}

bool Special::doesModifyActivator() const {
    return false;
}

void Special::switchOnOff() {
    _isActivated = !_isActivated;
}

std::string Special::getDynamicGroupHash() const {
    return "special;" + std::to_string(_position.at(0)) + "," + std::to_string(_position.at(1)) + ","
           + std::to_string(_position.at(2)) + "," + std::to_string(static_cast<int>(_direction));
}
