/*
 * File: Pyramid.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:19
 */

#ifndef PYRAMID_H
#define PYRAMID_H

#include "GeometricShape.h"

class Pyramid : public GeometricShape {
public:

    Pyramid(
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    Pyramid(
        const glm::vec3 &customColor,
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );


    virtual std::vector<glm::vec3> genPositions() const override;

    virtual std::vector<glm::vec3> genNormals() const override;

    virtual std::vector<glm::vec3> genColors(const std::vector<glm::vec3> &colors) const override;

private:
    static constexpr auto sharpHeight = 2.8f;
};

#endif /* PYRAMID_H */
