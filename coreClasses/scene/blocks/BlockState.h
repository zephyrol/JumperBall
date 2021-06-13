/*
 * File: BlockState.h
 * Author: Morgenthaler S
 *
 */
#ifndef BLOCKSTATE_H
#define BLOCKSTATE_H
#include <scene/blocks/Block.h>
#include "scene/items/ItemState.h"
#include <scene/ObjectState.h>

class BlockState:public ObjectState {
public:
BlockState(const Block& block);
virtual ObjectState::GlobalState update() override;
    
ObjectState::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

ObjectState::DynamicValues <float> getDynamicFloats() const override;
ObjectState::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
    
const bool& isFixed() const;

private:
const Block& _block;
JBTypes::vec3f _localScale;
JBTypes::vec3f _localTranslation;
bool _isUnited;
};

#endif // BLOCKSTATE_H
