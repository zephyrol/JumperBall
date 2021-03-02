/*
 * File: Cylinder.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:19
 */

#ifndef CYLINDER_H
#define CYLINDER_H
#include "GeometricShape.h"

class Cylinder:public GeometricShape {
public:

static constexpr size_t defaultMeriCount = 60;
// --CONSTRUCTORS & DESTRUCTORS--//
Cylinder(size_t meriCount = defaultMeriCount,
         const glm::mat4& modelTransform = glm::mat4(1.f),
         const glm::mat4& normalsTransform = glm::mat4(1.f));
Cylinder(const glm::vec3& customColor,
         size_t meriCount = defaultMeriCount,
         const glm::mat4& modelTransform = glm::mat4(1.f),
         const glm::mat4& normalsTransform = glm::mat4(1.f));
Cylinder(const glm::vec3& customColorCenter,
         const glm::vec3& customColorEdge,
         size_t meriCount = defaultMeriCount,
         const glm::mat4& modelTransform = glm::mat4(1.f),
         const glm::mat4& normalsTransform = glm::mat4(1.f));

virtual std::vector <glm::vec3> genPositions() const override;
virtual std::vector <glm::vec3> genNormals() const override;
virtual std::vector <glm::vec3> genColors(const std::vector <glm::vec3>& colors) const override;
virtual std::vector <glm::vec2> genUvCoords() const override;
virtual std::vector <GLushort> genIndices() const override;

private:

const size_t _meriCount;
const std::vector <glm::vec3> _customColors;
static const ShapeVertexAttributes basicInfoCylinder;


size_t levelOfDetail() const override;
// --------STATIC METHODS-------//
static ShapeVertexAttributes computeBasicInfoCylinder(
    size_t meriCount = defaultMeriCount);
static std::vector <glm::vec3> createCenterAndEdgeColorBuffer(
    const glm::vec3& customColorCenter,
    const glm::vec3& customColorEdge,
    size_t size
    );

};

#endif /* CYLINDER_H */
