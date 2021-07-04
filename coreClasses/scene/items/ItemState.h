/*
 * File: ItemState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
/*#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H
#include <scene/items/Item.h>
#include <scene/SceneElement.h>


class ItemState:public SceneElement {
public:
ItemState(const Item& item);

SceneElement::StaticValues <float> getStaticFloatValues() const override;
SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

SceneElement::DynamicValues <float> getDynamicFloats() const override;

SceneElement::SceneElementState update() override;

private:
const Item& _item;
const JBTypes::vec3ui& _position;
const float _direction;
float _timeSinceCreation;
float _timeSinceObtaining;

};

#endif // OBJECTSTATE_H
*/