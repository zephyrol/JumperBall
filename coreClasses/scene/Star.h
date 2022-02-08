/*
 * File: Star.h
 * Author: Morgenthlaler S
 *
 * Created on 20 décembre 2019, 13:46
 */

#ifndef STAR_H
#define STAR_H
#include "Map.h"

class Star: public SceneElement {
public:

// --CONSTRUCTORS & DESTRUCTORS--//
Star(
    const Map& map,
    float radiusInside,
    float radiusOutside,
    float distance,
    float radius,
    const JBTypes::vec3f& initialDirection,
    const JBTypes::vec3f& rotationAxis,
    float radiansPerSecond);

float radiusInside() const;
float radiusOutside() const;
float distance() const;
float radius() const;
const JBTypes::vec3f& rotationCenter() const;
float envSize() const;
const JBTypes::vec3f& initialDirection() const;
JBTypes::Quaternion getRotation() const;
JBTypes::vec3f lightDirection() const;
JBTypes::vec3f position() const;
SceneElement::GlobalState getGlobalState() const override;

    StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

SceneElement::DynamicValues <float> getDynamicFloats() const override;

static std::shared_ptr <Star> createBlurStar(const Map& map);

private:

// --------ATTRIBUTES-----------//
const float _radiusInside;
const float _radiusOutside;
const float _distance;
const float _radius;
const JBTypes::vec3f _initialDirection;
const JBTypes::vec3f _rotationAxis;
const JBTypes::vec3f _rotationCenter;
const float _envSize;
const float _radiansPerSeconds;
const JBTypes::timePointMs _creationTime;

float getTimeSinceCreation() const;

};

#endif /* STAR_H */
