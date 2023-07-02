/*
 * File: Star.h
 * Author: Morgenthaler S
 *
 * Created on 20 décembre 2019, 13:46
 */

#ifndef STAR_H
#define STAR_H

#include "Map.h"

class Star : public Displayable {
public:
    Star(
        const Map &map,
        float distance,
        const JBTypes::vec3f &initialDirection,
        const JBTypes::vec3f &rotationAxis,
        const JBTypes::vec3f &color,
        float radiansPerSecond
    );

    float distance() const;

    const JBTypes::vec3f &rotationCenter() const;

    float envSize() const;

    JBTypes::Quaternion getRotation() const;

    JBTypes::vec3f lightDirection() const;

    JBTypes::vec3f position() const;

    StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

    StaticValues<float> getStaticFloatValues() const override;

    Displayable::DynamicNames getDynamicFloatNames() const override;

    Displayable::DynamicValues<float> getDynamicFloatValues() const override;

    bool dynamicsMayChange() const override;

    static std::shared_ptr<Star> createBlurStar(const Map &map);

    static std::shared_ptr<Star> createPurpleStar(const Map &map);

private:

    const CstChronometer_sptr _chronometer;
    const float _distance;
    const JBTypes::vec3f _initialDirection;
    const JBTypes::vec3f _rotationAxis;
    const JBTypes::vec3f _rotationCenter;
    const float _envSize;
    const float _radiansPerSeconds;
    const JBTypes::vec3f _color;

};

#endif /* STAR_H */
