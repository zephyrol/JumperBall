/*
 * File: ItemState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H
#include <scene/items/Item.h>
#include <scene/State.h>


class ItemState:public State {
public:
ItemState(const Item& item);

State::StaticValues <float> getStaticFloatValues() const override;
State::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

State::DynamicValues <float> getDynamicFloats() const override;

State::GlobalState update() override;

private:
const Item& _item;
const JBTypes::vec3ui& _position;
const float _direction;
float _timeSinceCreation;
float _timeSinceObtaining;

};

#endif // OBJECTSTATE_H
