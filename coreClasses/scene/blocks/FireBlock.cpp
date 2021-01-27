
/* 
 * File:   FireBlock.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 octobre 2019, 12:50
 */

#include "FireBlock.h"

FireBlock::FireBlock(){
}

Block::Effect FireBlock::detectionEvent(const JBTypes::Dir&,
        const JBTypes::timePointMs&)
{
    return Block::Effect::Burn;
}
