/*
 * File: Cube.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 09:18
 */

#ifndef CUBE_H
#define CUBE_H
#include "GeometricShape.h"

class Cube:public GeometricShape {

public:

// --CONSTRUCTORS & DESTRUCTORS--//
Cube(const glm::mat4& modelTransform = glm::mat4(1.f),
     const glm::mat4& normalsTransform = glm::mat4(1.f),
     const std::array <bool, 6>& sides = { true, true, true, true, true, true }
     );
    Cube(const JBTypes::Color& color,
         const glm::mat4& modelTransform = glm::mat4(1.f),
         const glm::mat4& normalsTransform = glm::mat4(1.f),
         const std::array <bool, 6>& sides = { true, true, true, true, true, true }
    );
Cube(const glm::vec3& customColor,
     const glm::mat4& modelTransform = glm::mat4(1.f),
     const glm::mat4& normalsTransform = glm::mat4(1.f),
     const std::array <bool, 6>& sides = { true, true, true, true, true, true });
Cube(std::vector <glm::vec3>&& customColors,
     const glm::mat4& modelTransform = glm::mat4(1.f),
     const glm::mat4& normalsTransform = glm::mat4(1.f),
     const std::array <bool, 6>& sides = { true, true, true, true, true, true });

virtual std::vector <glm::vec3> genPositions() const override;
virtual std::vector <glm::vec3> genNormals() const override;
virtual std::vector <glm::vec3> genColors(const std::vector <glm::vec3>& colors) const override;

static const std::vector <glm::vec3> iceColorsCube;
static const std::vector <glm::vec3> fireColorsCube;
static const std::vector <glm::vec3> ghostColorsCube;
static const std::vector <glm::vec3> brittleColorsCube;

private:

const std::array <bool, 6> _sides;

template<typename T> static std::vector <T> getBasicElements(const std::vector <T>& basicElements,
                                                             const std::array <bool, 6>& sides);
static std::vector <glm::vec3> getBasicPosCube(const std::array <bool, 6>& sides);
static std::vector <glm::vec3> getBasicNormalsCube(const std::array <bool, 6>& sides);
static std::vector <glm::vec3> getBasicColorsCube(const std::array <bool, 6>& sides);
static std::vector <glm::vec2> getBasicUVCoordsCube(const std::array <bool, 6>& sides);

static std::vector <glm::vec3> genColors(const JBTypes::Color& color);
static const std::vector <glm::vec3> basicPositionsCube;
static const std::vector <glm::vec3> basicNormalsCube;
static const std::vector <glm::vec2> basicUVCoordsCube;
static const std::vector <glm::vec3> basicColorsCube;
};

#endif /* CUBE_H */
