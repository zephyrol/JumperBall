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
    if ( static_cast<int>(fPassedTime/_periodicity) % 2 == 0) {
        _isThere = true;
    } else {
        _isThere = false;
    }

    return Block::Effect::Nothing;
}
