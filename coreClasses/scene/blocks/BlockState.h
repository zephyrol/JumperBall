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

class BlockState: public State {
public:
BlockState(const Block& block);
virtual void update() override;

const std::array <float, 6>& localTransform() const;
const Block& block() const;
const std::array <std::shared_ptr <ObjectState>, 6>& objectStates() const;
std::vector <float> getDynamicFloats() const override;
std::vector <JBTypes::vec3f> getDynamicVec3fs() const override;

const bool& isFixed() const;

private:
const Block& _block;
std::array <float, 6> _localTransform;
std::array <std::shared_ptr <ObjectState>, 6> _objectStates;
const bool& _isFixed;

std::array <std::shared_ptr <ObjectState>, 6> createObjectStates() const;
};

#endif // BLOCKSTATE_H
