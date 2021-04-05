/*
 * File: Quad.h
 * Author: Morgenthaler S
 *
 * Created on 30 mars 2020, 18:34
 */

#ifndef QUAD_H
#define QUAD_H

#include "GeometricShape.h"

class Quad:public GeometricShape {
public:
Quad(const glm::mat4& modelTransform = glm::mat4(1.f),
     const glm::mat4& normalsTransform = glm::mat4(1.f));
Quad(const glm::vec3& customColor,
     const glm::mat4& modelTransform = glm::mat4(1.f),
     const glm::mat4& normalsTransform = glm::mat4(1.f));
Quad(const glm::vec3& customColor1,
     const glm::vec3& customColor2,
     const glm::mat4& modelTransform = glm::mat4(1.f),
     const glm::mat4& normalsTransform = glm::mat4(1.f));

virtual std::vector <glm::vec3> genPositions() const override;
virtual std::vector <glm::vec3> genNormals() const override;
virtual std::vector <glm::vec3> genColors(const std::vector <glm::vec3>& colors) const override;
virtual std::vector <glm::vec2> genUvCoords() const override;

private:
static std::vector <glm::vec3> createCustomDoubleColors(
    const glm::vec3& customColor1,
    const glm::vec3& customColor2);

static const std::vector <glm::vec3> basicPositionsQuad;
static const std::vector <glm::vec3> basicColorsQuad;
static const std::vector <glm::vec3> basicNormalsQuad;
static const std::vector <glm::vec2> basicUVCoordsQuad;
};


#endif /* QUAD_H */
