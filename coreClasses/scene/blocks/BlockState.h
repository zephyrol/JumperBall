/*
 * File: BlockState.h
 * Author: Morgenthaler S
 *
 */
/*#ifndef BLOCKSTATE_H
#define BLOCKSTATE_H
#include <scene/blocks/Block.h>
#include "scene/items/ItemState.h"
#include <scene/ObjectState.h>

class BlockState:public SceneElement {
public:
BlockState(const Block& block);
virtual SceneElement::SceneElementState update() override;

SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

SceneElement::DynamicValues <float> getDynamicFloats() const override;
SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;

const bool& isFixed() const;

private:
const Block& _block;
JBTypes::vec3f _localScale;
JBTypes::vec3f _localTranslation;
bool _isUnited;
};

#endif // BLOCKSTATE_H*/
