/*
 * File: Pyramid.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:19
 */

#include "Pyramid.h"

Pyramid::Pyramid(const glm::mat4& modelTransform,
                 const glm::mat4& normalsTransform)
    :GeometricShape(modelTransform, normalsTransform, {}) {

}

Pyramid::Pyramid(const glm::vec3& customColor,
                 const glm::mat4& modelTransform,
                 const glm::mat4& normalsTransform)
    :GeometricShape(modelTransform, normalsTransform, { customColor }) {

}

const std::vector <glm::vec3> Pyramid::basicPositionsPyramid =
    Utility::GLfloatListToGlmVec3({

    // Base
    0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
    // Face 1
    0.f, 0.f, 0.f, 0.5f, 1.f, 0.5f, 0.5f, 0.f, 0.f,
    0.5f, 0.f, 0.f, 0.5f, 1.f, 0.5f, 1.f, 0.f, 0.f,
    // Face 2
    0.5f, 0.f, 1.f, 0.5f, 1.f, 0.5f, 0.f, 0.f, 1.f,
    1.f, 0.f, 1.f, 0.5f, 1.f, 0.5f, 0.5f, 0.f, 1.f,
    // Face 3
    0.f, 0.f, 0.f, 0.f, 0.f, 0.5f, 0.5f, 1.f, 0.5f,
    0.f, 0.f, 0.5f, 0.f, 0.f, 1.f, 0.5f, 1.f, 0.5f,
    // Face 4
    1.f, 0.f, 0.5f, 1.f, 0.f, 0.f, 0.5f, 1.f, 0.5f,
    1.f, 0.f, 1.f, 1.f, 0.f, 0.5f, 0.5f, 1.f, 0.5f
});

const std::vector <glm::vec3> Pyramid::basicNormalsPyramid =
    Utility::GLfloatListToGlmVec3(Utility::computeNormals({
    // Base
    0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f,
    // Face 1
    0.f, 0.f, 0.f, 0.5f, 1.f, 0.5f, 0.5f, 0.f, 0.f,
    0.5f, 0.f, 0.f, 0.5f, 1.f, 0.5f, 1.f, 0.f, 0.f,
    // Face 2
    0.5f, 0.f, 1.f, 0.5f, 1.f, 0.5f, 0.f, 0.f, 1.f,
    1.f, 0.f, 1.f, 0.5f, 1.f, 0.5f, 0.5f, 0.f, 1.f,
    // Face 3
    0.f, 0.f, 0.f, 0.f, 0.f, 0.5f, 0.5f, 1.f, 0.5f,
    0.f, 0.f, 0.5f, 0.f, 0.f, 1.f, 0.5f, 1.f, 0.5f,
    // Face 4
    1.f, 0.f, 0.5f, 1.f, 0.f, 0.f, 0.5f, 1.f, 0.5f,
    1.f, 0.f, 1.f, 1.f, 0.f, 0.5f, 0.5f, 1.f, 0.5f
}));

const std::vector <glm::vec3> Pyramid::basicColorsPyramid =
    Utility::GLfloatListToGlmVec3({
    // Base
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    // Face 1
    0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.1f, 0.1f, 0.1f,
    // Face 2
    0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.1f, 0.1f, 0.1f,
    0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    // Face 3
    0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f, 0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f,
    // Face 4
    0.5f, 0.5f, 0.5f, 0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f,
    0.1f, 0.1f, 0.1f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
});

const std::vector <glm::vec2> Pyramid::basicUVCoordsPyramid =
    Utility::GLfloatListToGlmVec2({
    // Base
    0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 1.f, 0.f, 1.f, 0.f,
    // Face 1
    0.f, 0.f, 0.5f, 1.f, 0.5f, 0.f,
    0.5f, 0.f, 0.5f, 1.f, 1.f, 0.f,
    // Face 2
    0.5f, 0.f, 0.5f, 1.f, 0.f, 0.f,
    1.f, 0.f, 0.5f, 1.f, 0.5f, 0.f,
    // Face 3
    0.f, 0.f, 0.f, 0.f, 0.5f, 1.f,
    0.f, 0.f, 0.f, 0.f, 0.5f, 1.f,
    // Face 4
    1.f, 0.f, 1.f, 0.f, 0.5f, 1.f,
    1.f, 0.f, 1.f, 0.f, 0.5f, 1.f
});

std::vector <glm::vec3> Pyramid::genPositions() const {
    return basicPositionsPyramid;
}

std::vector <glm::vec3> Pyramid::genNormals() const {
    return basicNormalsPyramid;
}

std::vector <glm::vec3> Pyramid::genColors (const std::vector <glm::vec3>& colors) const {
    return colors.size() == 1
           ? GeometricShape::createCustomColorBuffer(colors.at(0), basicPositionsPyramid.size())
           : basicColorsPyramid;
}
