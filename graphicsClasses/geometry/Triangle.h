/*
 * File: Triangle.h
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:23
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GeometricShape.h"

class Triangle : public GeometricShape {
public:

    Triangle(const glm::mat4 &modelTransform = glm::mat4(1.f),
             const glm::mat4 &normalsTransform = glm::mat4(1.f));

    Triangle(const glm::vec3 &customColor,
             const glm::mat4 &modelTransform = glm::mat4(1.f),
             const glm::mat4 &normalsTransform = glm::mat4(1.f));

    virtual std::vector<glm::vec3> genPositions() const override;

    virtual std::vector<glm::vec3> genNormals() const override;

    virtual std::vector<glm::vec3> genColors(const std::vector<glm::vec3> &colors) const override;

    virtual std::vector<glm::vec2> genUvCoords(const std::vector<glm::vec2> &uvs) const override;

private:

    static std::vector<glm::vec3> createCustomColors(
        const glm::vec3 &customColor
    );

    static const std::vector<glm::vec3> basicPositionsTriangle;
    static const std::vector<glm::vec3> basicColorsTriangle;
    static const std::vector<glm::vec3> basicNormalsTriangle;
    static const std::vector<glm::vec2> basicUVCoordsTriangle;
};


#endif /* TRIANGLE_H */
