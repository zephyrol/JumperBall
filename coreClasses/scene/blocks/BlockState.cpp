/*
 * File: BlockState.cpp
 * Author: Morgenthaler S
 *
 */
#include "BlockState.h"

BlockState::BlockState(const Block& block):
    _block(block),
    _localTransform(block.localTransform()) {
}

State::GlobalState BlockState::update() {
    _localTransform = _block.localTransform();
    return State::GlobalState::United;
}

State::DynamicValues <float> BlockState::getDynamicFloats() const {
    const float scale = _localTransform.at(4);
    return {{ "scale", scale }};
}

State::DynamicValues <JBTypes::vec3f> BlockState::getDynamicVec3fs() const {
    const JBTypes::vec3f translation =
    { _localTransform.at(0), _localTransform.at(1), _localTransform.at(2) };
    return {{ "translation", translation }};
}
