/*
 * File: BlockState.h
 * Author: Morgenthaler S
 *
 */
#ifndef BLOCKSTATE_H
#define BLOCKSTATE_H
#include <scene/blocks/Block.h>
#include "scene/objects/ObjectState.h"
#include <scene/State.h>

class BlockState:public State {
public:
BlockState(const Block& block);
virtual State::GlobalState update() override;
    
State::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

State::DynamicValues <float> getDynamicFloats() const override;
State::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
    
const bool& isFixed() const;

private:
const Block& _block;
JBTypes::vec3f _localScale;
JBTypes::vec3f _localTranslation;
bool _isUnited;
};

#endif // BLOCKSTATE_H
