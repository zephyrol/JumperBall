/*
 * File: Sphere.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 14:43
 */

#ifndef SPHERE_H
#define SPHERE_H
#include "GeometricShape.h"


class Sphere:public GeometricShape {
public:

// --CONSTRUCTORS & DESTRUCTORS--//
Sphere(const glm::mat4& modelTransform = glm::mat4(1.f),
       const glm::mat4& normalsTransform = glm::mat4(1.f));
Sphere(const glm::vec3& customColor,
       const glm::mat4& modelTransform = glm::mat4(1.f),
       const glm::mat4& normalsTransform = glm::mat4(1.f));
Sphere(const glm::vec3& customColor,
       const glm::vec3& customColor2,
       const glm::mat4& modelTransform = glm::mat4(1.f),
       const glm::mat4& normalsTransform = glm::mat4(1.f));

virtual std::vector <glm::vec3> genPositions() const override;
virtual std::vector <glm::vec3> genNormals() const override;
virtual std::vector <glm::vec3> genColors(const std::vector <glm::vec3>& colors) const override;
virtual std::vector <glm::vec2> genUvCoords() const override;
virtual std::vector <GLushort> genIndices() const override;

private:

static const GeometricShape::ShapeVertexAttributes basicInfoSphere;

// --------STATIC METHODS-------//
static GeometricShape::ShapeVertexAttributes computeBasicInfoSphere(
    bool useCustomColors = false,
    const glm::vec3& firstColor = glm::vec3(0.f),
    const glm::vec3& secondColor = glm::vec3(0.f)
    );
};

#endif /* SPHERE_H */
