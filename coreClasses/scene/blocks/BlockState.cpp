/*
 * File: BlockState.cpp
 * Author: Morgenthaler S
 *
 */
#include "BlockState.h"

BlockState::BlockState(const Block& block):
    _block(block),
    _localTransform(block.localTransform()),
    _isFixed(block.isFixed()) {
}

void BlockState::update() {
    _localTransform = _block.localTransform();
}

const std::array <float, 6>& BlockState::localTransform() const {
    return _localTransform;
}

const Block& BlockState::block() const {
    return _block;
}

const bool& BlockState::isFixed() const {
    return _isFixed;
}

std::map <std::string, float> BlockState::getDynamicFloats() const {
    const float scale = _localTransform.at(4);
    return {{ "scale", scale }};
}

std::map <std::string, JBTypes::vec3f> BlockState::getDynamicVec3fs() const {
    const JBTypes::vec3f translation =
    { _localTransform.at(0), _localTransform.at(1), _localTransform.at(2) };
    return {{ "translation", translation }};
}
