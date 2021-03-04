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
virtual void update() override;

const std::array <float, 6>& localTransform() const;
const Block& block() const;
std::map <std::string, float> getDynamicFloats() const override;
std::map <std::string, JBTypes::vec3f> getDynamicVec3fs() const override;

const bool& isFixed() const;

private:
const Block& _block;
std::array <float, 6> _localTransform;
const bool& _isFixed;

};

#endif // BLOCKSTATE_H
