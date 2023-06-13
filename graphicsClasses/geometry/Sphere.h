/*
 * File: Sphere.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 14:43
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "GeometricShape.h"


class Sphere : public GeometricShape {
public:

    explicit Sphere(
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    explicit Sphere(
        const glm::vec3 &customColor,
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    explicit Sphere(
        const JBTypes::Color &color,
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    Sphere(
        const glm::vec3 &customColor,
        const glm::vec3 &customColor2,
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    Sphere(
        std::vector<glm::vec3> &&customColors,
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    std::vector<glm::vec3> genPositions() const override;

    std::vector<glm::vec3> genNormals() const override;

    std::vector<glm::vec3> genColors(const std::vector<glm::vec3> &colors) const override;

    std::vector<GLushort> genIndices() const override;

private:

    const bool _isHq;

    static glm::vec3 getSphereColor(const JBTypes::Color &color);

    static constexpr unsigned int sdIParaCount = 20;
    static constexpr unsigned int sdIMeriCount = 30;
    static constexpr unsigned int hqIParaCount = 40;
    static constexpr unsigned int hqIMeriCount = 60;

};

#endif /* SPHERE_H */
