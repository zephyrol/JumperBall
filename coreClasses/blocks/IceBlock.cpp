/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IceBlock.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 octobre 2019, 12:49
 */

#include "IceBlock.h"

IceBlock::IceBlock() {
}

Block::Effect IceBlock::detectionEvent(const JBTypes::Dir&,
        const JBTypes::timePointMs&)
{
    return Block::Effect::Slide;
}
