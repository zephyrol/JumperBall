/*
 * File: Pyramid.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:19
 */

#include "Pyramid.h"

Pyramid::Pyramid(
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform
)
    : GeometricShape(modelTransform, normalsTransform, {}, {}) {

}

Pyramid::Pyramid(
    const glm::vec3 &customColor,
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform
)
    : GeometricShape(modelTransform, normalsTransform, {customColor}, {}) {

}

std::vector<glm::vec3> Pyramid::genPositions() const {
    return Utility::GLfloatListToGlmVec3(
        {
            // Face 1
            -0.5f, 0.f, -0.5f, 0.f, sharpHeight, 0.f, 0.f, 0.f, -0.5f,
            0.f, 0.f, -0.5f, 0.f, sharpHeight, 0.f, 0.5f, 0.f, -0.5f,
            // Face 2
            0.f, 0.f, 0.5f, 0.f, sharpHeight, 0.f, -0.5f, 0.f, 0.5f,
            0.5f, 0.f, 0.5f, 0.f, sharpHeight, 0.f, 0.f, 0.f, 0.5f,
            // Face 3
            -0.5f, 0.f, -0.5f, -0.5f, 0.f, 0.f, 0.f, sharpHeight, 0.f,
            -0.5f, 0.f, 0.f, -0.5f, 0.f, 0.5f, 0.f, sharpHeight, 0.f,
            // Face 4
            0.5f, 0.f, 0.f, 0.5f, 0.f, -0.5f, 0.f, sharpHeight, 0.f,
            0.5f, 0.f, 0.5f, 0.5f, 0.f, 0.f, 0.f, sharpHeight, 0.f
        }
    );
}

std::vector<glm::vec3> Pyramid::genNormals() const {
    const glm::vec3 face1Normal = glm::normalize(glm::vec3{0.f, 1.f, -sharpHeight});
    const glm::vec3 face2Normal = glm::normalize(glm::vec3{0.f, 1.f, sharpHeight});
    const glm::vec3 face3Normal = glm::normalize(glm::vec3{-sharpHeight, 1.f, 0.f});
    const glm::vec3 face4Normal = glm::normalize(glm::vec3{sharpHeight, 1.f, 0.f});
    return {
        face1Normal, face1Normal, face1Normal, face1Normal, face1Normal, face1Normal,
        face2Normal, face2Normal, face2Normal, face2Normal, face2Normal, face2Normal,
        face3Normal, face3Normal, face3Normal, face3Normal, face3Normal, face3Normal,
        face4Normal, face4Normal, face4Normal, face4Normal, face4Normal, face4Normal
    };
}

std::vector<glm::vec3> Pyramid::genColors(const std::vector<glm::vec3> &colors) const {
    constexpr size_t numberOfColorValues = 72;
    return colors.size() == 1
           ? GeometricShape::createCustomColorBuffer(colors.at(0), numberOfColorValues)
           : Utility::GLfloatListToGlmVec3(
            {
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
}
