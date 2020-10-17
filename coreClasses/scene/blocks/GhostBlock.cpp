/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GhostBlock.cpp
 * Author: Morgenthaler S
 * 
 * Created on 20 aout 2020, 21:33
 */

#include "GhostBlock.h"

GhostBlock::GhostBlock(float periodicity):
Block(true,false),
_periodicity(periodicity),
_creationTime(JBTypesMethods::getTimePointMSNow()),
_isThere(true)
{
}

bool GhostBlock::stillExists() const
{
   return _isThere;
}

Block::Effect GhostBlock::interaction( const JBTypes::Dir& ,
                                        const JBTypes::timePointMs& currentTime,
                                        const JBTypes::vec3f& ,
                                        const std::array<unsigned int, 3>& ) {
    const auto passedTime = currentTime - _creationTime;
    const float fPassedTime =
            JBTypesMethods::getFloatFromDurationMS(passedTime);
    const unsigned int nbOfSwitching =
        static_cast<unsigned int>(fPassedTime/_periodicity);
    if (nbOfSwitching % 2 == 0) {
        _isThere = true;
    } else {
        _isThere = false;
    }

    const float passedTimeSinceSwitching =
        fPassedTime - _periodicity * static_cast<float>(nbOfSwitching);
    
    constexpr float animationTime = 0.2f;
    const float coeffPeriod = (passedTimeSinceSwitching /
                               animationTime);
    const float t = coeffPeriod > 1.f
    ? 1.f
    : coeffPeriod ;

    const float beginScale = _isThere
        ? 0.f
        : 1.f;
    const float endScale = _isThere
        ? 1.f
        : 0.f;
    
    const std::function<float(float,float,float)> getScale =
        [](float t, float begin, float end){
            return t * begin + (1-t) * end;
    };
    
    const float scale = getScale(t,beginScale,endScale);
    _localTransform.at(3) = scale;
    _localTransform.at(4) = scale;
    _localTransform.at(5) = scale;
    
    return Block::Effect::Nothing;
}
