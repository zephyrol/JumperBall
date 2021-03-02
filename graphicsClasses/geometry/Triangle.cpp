/*
 * File: Triangle.cpp
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:23
 */

#include "Triangle.h"

Triangle::Triangle(const glm::mat4& modelTransform,
                   const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, {}) {

}

Triangle::Triangle(const glm::vec3& customColor,
                   const glm::mat4& modelTransform,
                   const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, { customColor }) {

}

/*Triangle::Triangle(const glm::mat4& modelTransform,
                   const glm::mat4& normalsTransform):
    GeometricShape(
        basicPositionsTriangle,
        basicNormalsTriangle,
        basicColorsTriangle,
        basicUVCoordsTriangle,
        modelTransform,
        normalsTransform) {
   }

   Triangle::Triangle(const glm::vec3& customColor,
                   const glm::mat4& modelTransform,
                   const glm::mat4& normalsTransform):
    GeometricShape(
        basicPositionsTriangle,
        basicNormalsTriangle,
   b       GeometricShape::createCustomColorBuffer(customColor,
                                                basicPositionsTriangle.size()
                                                ),
        basicUVCoordsTriangle,
        modelTransform,
        normalsTransform) {
   }

   Triangle::Triangle(const GeometricShape& triangle,
                   const glm::mat4& modelTransform,
                   const glm::mat4& normalsTransform):
    GeometricShape(triangle, modelTransform, normalsTransform) {
   }*/

std::vector <glm::vec3> Triangle::createCustomColors (
    const glm::vec3& customColor
    ) {
    return Utility::GLfloatListToGlmVec3({
        customColor.r, customColor.g, customColor.b,
        customColor.r, customColor.g, customColor.b,
        customColor.r, customColor.g, customColor.b,
    });
}

const std::vector <glm::vec3> Triangle::basicPositionsTriangle =
    Utility::GLfloatListToGlmVec3({
    0.f, 1.f, 0.f, -1.f, -1.f, 0.f, 1.f, -1.f, 0.f
});

const std::vector <glm::vec3> Triangle::basicNormalsTriangle =
    Utility::GLfloatListToGlmVec3({
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f
});

const std::vector <glm::vec3> Triangle::basicColorsTriangle =
    Utility::GLfloatListToGlmVec3({
    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f
});

const std::vector <glm::vec2> Triangle::basicUVCoordsTriangle =
    Utility::GLfloatListToGlmVec2({
    0.f, 1.f, 0.f, 0.f, 0.f, 0.f
});

std::vector <glm::vec3> Triangle::genPositions() const {
    return basicPositionsTriangle;
}

std::vector <glm::vec3> Triangle::genNormals() const {
    return basicNormalsTriangle;
}

std::vector <glm::vec3> Triangle::genColors (const std::vector <glm::vec3>& colors) const {
    if (colors.size() > 0) {

    }
    return colors.size() > 0
           ? GeometricShape::createCustomColorBuffer(colors.at(0), basicPositionsTriangle.size())
           : basicColorsTriangle;
}

std::vector <glm::vec2> Triangle::genUvCoords() const {
    return basicUVCoordsTriangle;
}
