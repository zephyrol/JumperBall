/*
 * File: Item.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "system/Types.h"
#include "scene/SceneElement.h"

class Item : public SceneElement{
public:

enum class CategoryOfItems { Key, Coin, Clock };

Item(const JBTypes::vec3ui& position, const JBTypes::Dir& direction, const CategoryOfItems& category);
virtual ~Item() = default;
const CategoryOfItems& getCategory() const;
const JBTypes::vec3ui& position() const;
const JBTypes::Dir& direction() const;

bool isGotten() const;
float getTimeSinceObtaining() const;
float getTimeSinceCreation() const;
void catchingTest(const JBTypes::vec3f& itemPosition,
                  const JBTypes::vec3f& entityPosition,
                  float radiusEntity);

static constexpr float radiusBoundingSphere = 0.15f;

SceneElement::StaticValues <float> getStaticFloatValues() const override;
SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;
SceneElement::DynamicValues <float> getDynamicFloats() const override;

SceneElement::GlobalState getGlobalState() const override;

private:
const JBTypes::vec3ui _position;
const JBTypes::Dir _direction;
bool _gotten;
const JBTypes::timePointMs _creationTime;
JBTypes::timePointMs _obtainingTime;
const CategoryOfItems _category;

};

#endif /* OBJECT_H */
