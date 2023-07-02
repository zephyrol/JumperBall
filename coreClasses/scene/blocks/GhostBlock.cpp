/*
 * File: GhostBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 aout 2020, 21:33
 */

#include "GhostBlock.h"

GhostBlock::GhostBlock(
    const JBTypes::vec3ui &position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball,
    float periodicity
) :
    InteractiveBlock(position, items, enemies, specials, ball, true),
    _chronometer(ball->getInGameChronometer()),
    _periodicity(periodicity),
    _isThere(true) {
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

    const std::function<float(float, float, float)> getScale =
        [](float t, float begin, float end) {
            return t * begin + (1 - t) * end;
        };

    const float scale = getScale(t, beginScale, endScale);
    // const float scale = _isThere ? 1.f : 0.f;
    _localScale = {scale, scale, scale};

    const float translation = (1 - scale) * (-0.5f);
    _localTranslation = {translation, translation, translation};
}

Displayable::StaticValues<JBTypes::vec3f> GhostBlock::getStaticVec3fValues() const {
    // We need to move the position to the center of the block to get
    // scale animation
    constexpr float offset = 0.5f;
    return {{
                static_cast <float>(_position.at(0)) + offset,
                static_cast <float>(_position.at(1)) + offset,
                static_cast <float>(_position.at(2)) + offset
            }};
}

std::string GhostBlock::getDynamicGroupHash() const {
    return "ghost;" + std::to_string(_periodicity);
}
