/*
 * File: Quad.h
 * Author: Morgenthaler S
 *
 * Created on 30 mars 2020, 18:34
 */

#ifndef QUAD_H
#define QUAD_H

#include "GeometricShape.h"

class Quad : public GeometricShape {
public:
    Quad(
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f),
        std::vector<glm::vec2> &&uvs = {}
    );

    Quad(
        const glm::vec3 &customColor,
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f),
        std::vector<glm::vec2> &&uvs = {}
    );

    Quad(
        const glm::vec3 &customColor1,
        const glm::vec3 &customColor2,
        const glm::mat4 &modelTransform = glm::mat4(1.f),
        const glm::mat4 &normalsTransform = glm::mat4(1.f)
    );

    virtual std::vector<glm::vec3> genPositions() const override;

    virtual std::vector<glm::vec3> genColors(const std::vector<glm::vec3> &colors) const override;

    virtual std::vector<glm::vec2> genUvCoords(const std::vector<glm::vec2> &uvs) const override;

private:
    static std::vector<glm::vec3> createCustomDoubleColors(
        const glm::vec3 &customColor1,
        const glm::vec3 &customColor2
    );
};


#endif /* QUAD_H */
