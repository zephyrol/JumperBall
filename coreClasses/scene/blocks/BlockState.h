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

State::DynamicValues <float> getDynamicFloats() const override;
State::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;

const bool& isFixed() const;

private:
const Block& _block;
std::array <float, 6> _localTransform;

};

#endif // BLOCKSTATE_H
