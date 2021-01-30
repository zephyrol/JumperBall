/*
 * File: BlockState.cpp
 * Author: Morgenthaler S
 *
 */
#include "BlockState.h"

BlockState::BlockState(const Block& block):
    _block(block),
    _localTransform(block.localTransform()),
    _objectStates(createObjectStates()),
    _isFixed(block.isFixed()) {
}

void BlockState::update() {
    _localTransform = _block.localTransform();
    for (std::shared_ptr <ObjectState>& ObjectState : _objectStates) {
        if (ObjectState) {
            ObjectState->update();
        }
    }
}

const std::array <float, 6>& BlockState::localTransform() const {
    return _localTransform;
}

const Block& BlockState::block() const {
    return _block;
}

const std::array <std::shared_ptr <ObjectState>, 6>& BlockState::objectStates()
const {
    return _objectStates;
}

const bool& BlockState::isFixed() const {
    return _isFixed;
}

std::array <std::shared_ptr <ObjectState>, 6> BlockState::createObjectStates()
const {
    std::array <std::shared_ptr <ObjectState>, 6> objectStates;

    for (size_t i = 0; i < _block.objects().size(); ++i) {
        if (const auto& object = _block.objects().at(i)) {
            objectStates.at(i) = std::make_shared <ObjectState>(*object);
        }
    }

    return objectStates;
}
