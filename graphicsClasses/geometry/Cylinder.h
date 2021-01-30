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
Cylinder(const GeometricShape& cylinder,
         const glm::mat4& modelTransform = glm::mat4(1.f),
         const glm::mat4& normalsTransform = glm::mat4(1.f));


// ------------TYPES------------//
struct InfoCylinder {
    InfoCylinder():positions{}, normals{}, colors{}, uvCoords{}, indices{} {
    }
    std::vector <glm::vec3> positions;
    std::vector <glm::vec3> normals;
    std::vector <glm::vec3> colors;
    std::vector <glm::vec2> uvCoords;
    std::vector <GLushort> indices;
};

private:

const size_t _meriCount;
static const InfoCylinder basicInfoCylinder;


size_t levelOfDetail() const override;
// --------STATIC METHODS-------//
static InfoCylinder computeBasicInfoCylinder(
    size_t meriCount = defaultMeriCount);
static std::vector <glm::vec3> createCenterAndEdgeColorBuffer(
    const glm::vec3& customColorCenter,
    const glm::vec3& customColorEdge,
    size_t size
    );

};

#endif /* CYLINDER_H */
