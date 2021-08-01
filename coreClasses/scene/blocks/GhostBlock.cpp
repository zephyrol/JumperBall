/*
 * File: GhostBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 aout 2020, 21:33
 */

#include "GhostBlock.h"
#include <functional>

GhostBlock::GhostBlock(const JBTypes::vec3ui& position, float periodicity):
    Block(position, true, false),
    _periodicity(periodicity),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _isThere(true) {
}

bool GhostBlock::isExists() const {
    return _isThere;
}

Block::Effect GhostBlock::interaction (const JBTypes::Dir&,
                                       const JBTypes::timePointMs& currentTime,
                                       const JBTypes::vec3f&
                                       ) {
    const auto passedTime = currentTime - _creationTime;
    const float fPassedTime = JBTypesMethods::getFloatFromDurationMS(passedTime);
    const unsigned int nbOfSwitching = static_cast <unsigned int>(fPassedTime / _periodicity);
    if (nbOfSwitching % 2 == 0) {
        _isThere = true;
    } else {
        _isThere = false;
    }

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

std::string GhostBlock::getBlockOptions() const {
    const unsigned int periodicityUint = static_cast<unsigned int>(_periodicity);
    return std::to_string(periodicityUint);
}
