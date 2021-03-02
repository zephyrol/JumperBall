/*
 * File: Quad.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 mars 2020, 18:34
 */

#include "Quad.h"

Quad::Quad(const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, {}) {

}

Quad::Quad(const glm::vec3& customColor,
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, { customColor }) {

}

Quad::Quad(const glm::vec3& customColor1,
           const glm::vec3& customColor2,
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(modelTransform, normalsTransform, { customColor1, customColor2 }) {

}

/*Quad::Quad(const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(
        basicPositionsQuad,
        basicNormalsQuad,
        basicColorsQuad,
        basicUVCoordsQuad,
        modelTransform,
        normalsTransform) {
   }

   Quad::Quad(const glm::vec3& customColor,
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(
        basicPositionsQuad,
        basicNormalsQuad,
        GeometricShape::createCustomColorBuffer(customColor,
                                                basicPositionsQuad.size()),
        basicUVCoordsQuad,
        modelTransform,
        normalsTransform) {
   }

   Quad::Quad(const glm::vec3& customColor1,
           const glm::vec3& customColor2,
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(
        basicPositionsQuad,
        basicNormalsQuad,
        createCustomDoubleColors(customColor1, customColor2),
        basicUVCoordsQuad,
        modelTransform,
        normalsTransform) {
   }*/


/*Quad::Quad(const GeometricShape& quad,
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(quad, modelTransform, normalsTransform) {
   }*/

std::vector <glm::vec3> Quad::createCustomDoubleColors (
    const glm::vec3& customColor1, const glm::vec3& customColor2) {
    return Utility::GLfloatListToGlmVec3({
        customColor2.r, customColor2.g, customColor2.b,
        customColor1.r, customColor1.g, customColor1.b,
        customColor2.r, customColor2.g, customColor2.b,

        customColor2.r, customColor2.g, customColor2.b,
        customColor2.r, customColor2.g, customColor2.b,
        customColor1.r, customColor1.g, customColor1.b
    });
}


const std::vector <glm::vec3> Quad::basicPositionsQuad =
    Utility::GLfloatListToGlmVec3({
    -1.f, 1.f, 0.f, -1.f, -1.f, 0.f, 1.f, -1.f, 0.f,
    -1.f, 1.f, 0.f, 1.f, -1.f, 0.f, 1.f, 1.f, 0.f
});

const std::vector <glm::vec3> Quad::basicNormalsQuad =
    Utility::GLfloatListToGlmVec3({
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f,
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f
});

const std::vector <glm::vec3> Quad::basicColorsQuad =
    Utility::GLfloatListToGlmVec3({
    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f
});

const std::vector <glm::vec2> Quad::basicUVCoordsQuad =
    Utility::GLfloatListToGlmVec2({
    0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
    0.f, 1.f, 1.f, 0.f, 1.f, 1.f
});

std::vector <glm::vec3> Quad::genPositions() const {
    return basicPositionsQuad;
}

std::vector <glm::vec3> Quad::genNormals() const {
    return basicNormalsQuad;
}

std::vector <glm::vec3> Quad::genColors (const std::vector <glm::vec3>& colors) const {
    if (colors.size() == 2) {
        return createCustomDoubleColors(colors.at(0), colors.at(1));
    }
    if (colors.size() == 1) {
        return GeometricShape::createCustomColorBuffer(colors.at(0), basicPositionsQuad.size());
    }
    return basicColorsQuad;
}

std::vector <glm::vec2> Quad::genUvCoords() const {
    return basicUVCoordsQuad;
}
