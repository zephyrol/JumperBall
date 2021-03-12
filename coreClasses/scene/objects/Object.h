/*
 * File: Object.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "system/Types.h"

class Object {
public:

enum class CategoryOfObjects { Key, Coin, Clock };

Object(const JBTypes::vec3ui& position, const JBTypes::Dir& direction, const CategoryOfObjects& category);
virtual ~Object() = default;
const CategoryOfObjects& getCategory() const;
const JBTypes::vec3ui& position() const;
const JBTypes::Dir& direction() const;

bool isGotten() const;
float getTimeSinceObtaining() const;
float getTimeSinceCreation() const;
void catchingTest(const JBTypes::vec3f& objectPosition,
                  const JBTypes::vec3f& entityPosition,
                  float radiusEntity);

static constexpr float radiusBoundingSphere = 0.15f;

private:
const JBTypes::vec3ui _position;
const JBTypes::Dir _direction;
bool _gotten;
const JBTypes::timePointMs _creationTime;
JBTypes::timePointMs _obtainingTime;
const CategoryOfObjects _category;
};

#endif /* OBJECT_H */
