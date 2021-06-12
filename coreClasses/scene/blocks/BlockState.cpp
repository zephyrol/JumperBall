/*
 * File: BlockState.cpp
 * Author: Morgenthaler S
 *
 */
#include "BlockState.h"

BlockState::BlockState(const Block& block):
    _block(block),
    _localScale(block.localScale()),
    _localTranslation(block.localTranslation()),
    _isUnited(block.isExists())
{
}

ObjectState::GlobalState BlockState::update() {
    _localScale = _block.localScale();
    _localTranslation = _block.localTranslation();
    if (_isUnited) {
        _isUnited = _block.isExists();
    }
    return _isUnited
      ? ObjectState::GlobalState::United
      : ObjectState::GlobalState::Separate;
}

ObjectState::StaticValues <JBTypes::vec3f> BlockState::getStaticVec3fValues() const {
    const auto& position = _block.position();
    constexpr float offset = 0.5f;
    return {{
        static_cast<float>(position.at(0) + offset),
        static_cast<float>(position.at(1) + offset),
        static_cast<float>(position.at(2) + offset) 
    }};
}

ObjectState::DynamicValues <float> BlockState::getDynamicFloats() const {
    return {};
}

ObjectState::DynamicValues <JBTypes::vec3f> BlockState::getDynamicVec3fs() const {
    return { { "translation", _localTranslation }, { "scale", _localScale } };
}
