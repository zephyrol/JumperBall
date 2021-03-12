/*
 * File: Star.h
 * Author: Morgenthlaler S
 *
 * Created on 20 décembre 2019, 13:46
 */

#ifndef STAR_H
#define STAR_H
#include "system/Types.h"

class Star {
public:

// --CONSTRUCTORS & DESTRUCTORS--//
Star(
    float radiusInside,
    float radiusOutside,
    float distance,
    float radius,
    const JBTypes::vec3f& rotationCenter);

float radiusInside() const;
float radiusOutside() const;
float distance() const;
float radius() const;
float getTimeSinceCreation() const;
const JBTypes::vec3f& rotationCenter() const;
JBTypes::vec3f lightDirection() const;

private:

// --------ATTRIBUTES-----------//
const float _radiusInside;
const float _radiusOutside;
const float _distance;
const float _radius;
const JBTypes::vec3f _rotationCenter;
const JBTypes::timePointMs _creationTime;

};

#endif /* STAR_H */
