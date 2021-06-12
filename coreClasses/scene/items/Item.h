/*
 * File: Item.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "system/Types.h"

class Item {
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

private:
const JBTypes::vec3ui _position;
const JBTypes::Dir _direction;
bool _gotten;
const JBTypes::timePointMs _creationTime;
JBTypes::timePointMs _obtainingTime;
const CategoryOfItems _category;
};

#endif /* OBJECT_H */
