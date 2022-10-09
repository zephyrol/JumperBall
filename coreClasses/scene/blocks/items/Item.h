/*
 * File: Item.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "system/Types.h"
#include "scene/Displayable.h"
#include <scene/Shape.h>

class Item;
using Item_sptr = std::shared_ptr <Item>;
using CstItem_sptr = std::shared_ptr <const Item>;
using vecCstItem_sptr = std::vector <CstItem_sptr>;
using vecItem_sptr = std::vector <Item_sptr>;
using Item_uptr = std::unique_ptr <Item>;

class Item : public Displayable{
public:

Item(const JBTypes::vec3ui& position, const JBTypes::Dir& direction);
const JBTypes::vec3ui& position() const;
const JBTypes::vec3f& get3DPosition() const;
const JBTypes::Dir& direction() const;

bool isGotten() const;
float getTimeSinceObtaining() const;
float getTimeSinceCreation() const;
virtual void catchingTest(const JBTypes::timePointMs &updatingTime) = 0;

virtual vecCstShape_sptr getShapes() const = 0;

static constexpr float itemBoundingSphereRadius = 0.15f;

Displayable::StaticValues <float> getStaticFloatValues() const override;
Displayable::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;
Displayable::DynamicValues <float> getDynamicFloats() const override;

Displayable::GlobalState getGlobalState() const override;
bool globalStateMayChange() const override;

void setAsGotten(const JBTypes::timePointMs &updatingTime);

private:

JBTypes::vec3f compute3DPosition() const;

const JBTypes::vec3ui _position;
const JBTypes::Dir _direction;
const JBTypes::vec3f _3Dposition;
bool _gotten;
const JBTypes::timePointMs _creationTime;
JBTypes::timePointMs _obtainingTime;

};

#endif /* OBJECT_H */
