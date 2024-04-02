/*
 * File: Star.h
 * Author: Morgenthaler S
 *
 * Created on 20 décembre 2019, 13:46
 */

#ifndef STAR_H
#define STAR_H

#include "Map.h"

class Star;
using CstStar_sptr = std::shared_ptr<const Star>;

class Star : public Displayable {
public:
    Star(
        const Map &map,
        float distance,
        const glm::vec3 &initialDirection,
        const glm::vec3 &rotationAxis,
        const glm::vec3 &color,
        float radiansPerSecond
    );

    const glm::vec3 &rotationCenter() const;

    float envSize() const;

    glm::quat getRotation() const;

    glm::vec3 lightDirection() const;

    glm::vec3 position() const;

    StaticValues <glm::vec3> getStaticVec3fValues() const override;

    StaticValues<float> getStaticFloatValues() const override;

    Displayable::DynamicNames getDynamicFloatNames() const override;

    Displayable::DynamicValues<float> getDynamicFloatValues() const override;

    static std::shared_ptr<Star> createBlurStar(const Map &map);

    static std::shared_ptr<Star> createPurpleStar(const Map &map);

private:

    const CstChronometer_sptr _chronometer;
    const float _distance;
    const glm::vec3 _initialDirection;
    const glm::vec3 _rotationAxis;
    const glm::vec3 _rotationCenter;
    const float _envSize;
    const float _radiansPerSeconds;
    const glm::vec3 _color;

};

#endif /* STAR_H */
