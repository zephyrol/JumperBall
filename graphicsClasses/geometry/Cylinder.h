/*
 * File: Cylinder.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:19
 */

#ifndef CYLINDER_H
#define CYLINDER_H

#include "GeometricShape.h"

class Cylinder : public GeometricShape {
public:

    static constexpr size_t defaultMeriCount = 60;

    Cylinder(size_t meriCount = defaultMeriCount,
             const glm::mat4 &modelTransform = glm::mat4(1.f),
             const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    Cylinder(const glm::vec3 &customColor,
             size_t meriCount = defaultMeriCount,
             const glm::mat4 &modelTransform = glm::mat4(1.f),
             const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    Cylinder(const JBTypes::Color &color,
             size_t meriCount = defaultMeriCount,
             const glm::mat4 &modelTransform = glm::mat4(1.f),
             const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    Cylinder(const glm::vec3 &customColorCenter,
             const glm::vec3 &customColorEdge,
             size_t meriCount = defaultMeriCount,
             const glm::mat4 &modelTransform = glm::mat4(1.f),
             const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    std::vector<glm::vec3> genPositions() const override;

    std::vector<glm::vec3> genNormals() const override;

    std::vector<glm::vec3> genColors(const std::vector<glm::vec3> &colors) const override;

    std::vector<GLushort> genIndices() const override;

private:
    const size_t _meriCount;
    const std::vector<glm::vec3> _customColors;

    struct BasicInfo {
        std::vector<glm::vec3> positions {};
        std::vector<glm::vec3> normals {};
        std::vector<glm::vec3> colors {};
        std::vector<GLushort> indices {};
    };

    static BasicInfo computeBasicInfoCylinder(size_t meriCount = defaultMeriCount);

    static std::vector<glm::vec3> createCenterAndEdgeColorBuffer(
        const glm::vec3 &customColorCenter,
        const glm::vec3 &customColorEdge,
        size_t size
    );

    static std::array<glm::vec3, 2> getCenterAndEdgeColor(const JBTypes::Color &color);

};

#endif /* CYLINDER_H */
