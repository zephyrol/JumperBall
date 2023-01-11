/*
 * File: Quad.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 mars 2020, 18:34
 */

#include "Quad.h"

Quad::Quad(
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform,
    std::vector<glm::vec2> &&uvs
) :
    GeometricShape(modelTransform, normalsTransform, {}, std::move(uvs)) {
}

Quad::Quad(
    const glm::vec3 &customColor,
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform,
    std::vector<glm::vec2> &&uvs
) :
    GeometricShape(modelTransform, normalsTransform, {customColor}, std::move(uvs)) {
}

Quad::Quad(
    const glm::vec3 &customColor1,
    const glm::vec3 &customColor2,
    const glm::mat4 &modelTransform,
    const glm::mat4 &normalsTransform
) :
    GeometricShape(modelTransform, normalsTransform, {customColor1, customColor2}, {}) {
}

std::vector<glm::vec3> Quad::createCustomDoubleColors(
    const glm::vec3 &customColor1, const glm::vec3 &customColor2
) {
    return Utility::GLfloatListToGlmVec3(
        {
            customColor2.r, customColor2.g, customColor2.b,
            customColor1.r, customColor1.g, customColor1.b,
            customColor2.r, customColor2.g, customColor2.b,
            customColor2.r, customColor2.g, customColor2.b,
            customColor2.r, customColor2.g, customColor2.b,
            customColor1.r, customColor1.g, customColor1.b
        }
    );
}


std::vector<glm::vec3> Quad::genPositions() const {
    return Utility::GLfloatListToGlmVec3(
        {
            -1.f, 1.f, 0.f, -1.f, -1.f, 0.f, 1.f, -1.f, 0.f,
            -1.f, 1.f, 0.f, 1.f, -1.f, 0.f, 1.f, 1.f, 0.f
        }
    );
}

std::vector<glm::vec3> Quad::genColors(const std::vector<glm::vec3> &colors) const {
    if (colors.size() == 2) {
        return createCustomDoubleColors(colors.at(0), colors.at(1));
    }
    if (colors.size() == 1) {
        constexpr size_t numberOfFloatPositions = 18;
        return GeometricShape::createCustomColorBuffer(colors.at(0), numberOfFloatPositions);
    }
    return {};
}

std::vector<glm::vec2> Quad::genUvCoords(const std::vector<glm::vec2> &uvs) const {
    return uvs.empty()
           ? Utility::GLfloatListToGlmVec2(
            {
                0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
                0.f, 1.f, 1.f, 0.f, 1.f, 1.f
            })
           : uvs;
}
