/*
 * File: ItemState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H
#include <scene/items/Item.h>
#include <scene/ObjectState.h>


class ItemState:public ObjectState {
public:
ItemState(const Item& item);

ObjectState::StaticValues <float> getStaticFloatValues() const override;
ObjectState::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

ObjectState::DynamicValues <float> getDynamicFloats() const override;

ObjectState::GlobalState update() override;

private:
const Item& _item;
const JBTypes::vec3ui& _position;
const float _direction;
float _timeSinceCreation;
float _timeSinceObtaining;

};

#endif // OBJECTSTATE_H
