/*
 * File: GhostBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 aout 2020, 21:33
 */

#include "GhostBlock.h"

GhostBlock::GhostBlock(
    const glm::u32vec3 &position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball,
    float periodicity
) :
    InteractiveBlock(position, items, enemies, specials, ball, true),
    _chronometer(ball->getInGameChronometer()),
    _periodicity(periodicity),
    _fPosition({
        static_cast<float>(position.at(0)),
        static_cast<float>(position.at(1)),
        static_cast<float>(position.at(2))
    }),
    _isThere(true),
    _scale(1.f){
}

bool GhostBlock::isExists() const {
    return _isThere;
}

bool GhostBlock::mayDisappear() const {
    return true;
}

JBTypes::Color GhostBlock::getColor() const {
    return JBTypes::Color::Purple;
}

void GhostBlock::update() {
    InteractiveBlock::update();

    const auto fPassedTime = _chronometer->getTime();
    const auto nbOfSwitching = static_cast <unsigned int>(fPassedTime / _periodicity);
    _isThere = nbOfSwitching % 2 == 0;

    const float passedTimeSinceSwitching = fPassedTime - _periodicity * static_cast <float>(nbOfSwitching);

    constexpr float animationTime = 0.1f;
    const float coeffPeriod = (passedTimeSinceSwitching / animationTime);
    const float t = coeffPeriod > 1.f
                    ? 1.f
                    : coeffPeriod;

    const float beginScale = _isThere
                             ? 1.f
                             : 0.f;
    const float endScale = _isThere
                           ? 0.f
                           : 1.f;

    _scale = t * beginScale + (1.f - t) * endScale;
}

std::string GhostBlock::getDynamicGroupHash() const {
    return "ghost;" + std::to_string(position().at(0)) + "," +
           std::to_string(position().at(1)) + "," + std::to_string(position().at(2));
}

Displayable::DynamicValues<glm::vec3> GhostBlock::getDynamicVec3fValues() const {
    return {_fPosition, {_scale, _scale, _scale}};
}
