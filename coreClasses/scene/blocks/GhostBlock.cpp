/*
 * File: GhostBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 aout 2020, 21:33
 */

#include "GhostBlock.h"
#include <functional>

GhostBlock::GhostBlock(const JBTypes::vec3ui &position,
                       const vecItem_sptr &items,
                       const vecEnemy_sptr &enemies,
                       const vecSpecial_sptr &specials,
                       const Ball_sptr& ball,
                       float periodicity):
    InteractiveBlock(position, items, enemies, specials, ball, true, false),
    _periodicity(periodicity),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _isThere(true) {
}

bool GhostBlock::isExists() const {
    return _isThere;
}

Block::Effect GhostBlock::interaction (const JBTypes::timePointMs& currentTime) {
    const auto passedTime = currentTime - _creationTime;
    const float fPassedTime = JBTypesMethods::getFloatFromDurationMS(passedTime);
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

    const std::function <float(float, float, float)> getScale =
        [] (float t, float begin, float end) {
            return t * begin + (1 - t) * end;
        };

    const float scale = getScale(t, beginScale, endScale);
    // const float scale = _isThere ? 1.f : 0.f;
    _localScale = { scale, scale, scale };

    return Block::Effect::Nothing;
}

bool GhostBlock::mayDisappear() const {
    return true;
}

JBTypes::Color GhostBlock::getColor() const {
    return JBTypes::Color::Purple;
}

