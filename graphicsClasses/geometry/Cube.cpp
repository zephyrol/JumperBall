/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cube.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 29 mars 2020, 09:18
 */

#include "Cube.h"

Cube::Cube( const glm::mat4& modelTransform,
            const glm::mat4& normalsTransform,
            const std::array<bool,6>& sides):
    GeometricShape (
        getBasicPosCube(sides),
        getBasicNormalsCube(sides),
        getBasicColorsCube(sides),
        basicUVCoordsCube,
        modelTransform,
        normalsTransform) {
}

Cube::Cube( const glm::vec3& customColor,
            const glm::mat4& modelTransform,
            const glm::mat4& normalsTransform,
            const std::array<bool,6>& sides):
    GeometricShape (
        getBasicPosCube(sides),
        getBasicNormalsCube(sides),
        getBasicElements(GeometricShape::createCustomColorBuffer
                         (customColor, basicPositionsCube.size()), sides),
        basicUVCoordsCube,
        modelTransform,
        normalsTransform)
{
}


Cube::Cube( const std::vector<glm::vec3>& customColors,
            const std::array<bool,6>& sides,
            const glm::mat4& modelTransform,
            const glm::mat4& normalsTransform):
    GeometricShape (
        getBasicPosCube(sides),
        getBasicNormalsCube(sides),
        getBasicElements(customColors,sides),
        basicUVCoordsCube,
        modelTransform,
        normalsTransform)
{
}


Cube::Cube(const GeometricShape& cube,
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform):
    GeometricShape(cube,modelTransform,normalsTransform) {
    }

const std::vector<glm::vec3> Cube::getBasicPosCube(const std::array<bool, 6> &sides) {
    return getBasicElements(basicPositionsCube, sides);
}

const std::vector<glm::vec3> Cube::getBasicNormalsCube
    (const std::array<bool, 6> &sides) {
    return getBasicElements(basicNormalsCube, sides);
}

const std::vector<glm::vec3> Cube::getBasicColorsCube
(const std::array<bool, 6> &sides) {
    return getBasicElements(basicColorsCube, sides);
}

const std::vector<glm::vec3> Cube::getBasicElements
    (const std::vector<glm::vec3>& basicElements,
     const std::array<bool, 6> &sides){
    constexpr size_t vec3PerFace = 6;
    constexpr size_t nbFaces = 6;
    bool isValid;
    if (basicElements.size() != vec3PerFace * nbFaces ) {
        std::cerr << "Error: your buffer containing cube information " <<
        "not the good size (yours : " << basicElements.size() <<
        " vec3, required : " << vec3PerFace * nbFaces << std::endl <<
        "returns vector of 0.f ..." << std::endl;
        isValid = false;
    } else {
        isValid = true;
    }
    std::vector<glm::vec3> elements{};
    for (unsigned int i = 0; i < sides.size(); ++i) {
        if(sides.at(i) && isValid) {
            elements.insert(elements.end(),
                            basicElements.begin() + vec3PerFace * i,
                            basicElements.begin() + vec3PerFace* (i + 1));
        } else if ( sides.at(i) && !isValid) {
            for (unsigned int i = 0; i < vec3PerFace; ++i) {
                elements.push_back(glm::vec3(0.f,0.f,0.f));
            }
        }
    }
    return elements;
}

// order : 1 2 6 5 4 3

const std::vector<glm::vec3> Cube::basicPositionsCube =
Utility::GLfloatListToGlmVec3({
    //Face 1 
    0.f,1.f,0.f, 1.f,0.f,0.f,  0.f,0.f,0.f,
    0.f,1.f,0.f, 1.f,1.f,0.f,  1.f,0.f,0.f,
    //Face 2
    0.f,0.f,1.f, 1.f,0.f,1.f, 0.f,1.f,1.f,
    1.f,0.f,1.f, 1.f,1.f,1.f, 0.f,1.f,1.f,
    //Face 6
    1.f,0.f,0.f, 1.f,1.f,0.f, 1.f,0.f,1.f,
    1.f,1.f,0.f, 1.f,1.f,1.f, 1.f,0.f,1.f,
    //Face 5
    0.f,0.f,1.f, 0.f,1.f,0.f,  0.f,0.f,0.f,
    0.f,0.f,1.f, 0.f,1.f,1.f,  0.f,1.f,0.f,
    //Face 4
    0.f,1.f,1.f, 1.f,1.f,0.f,  0.f,1.f,0.f,
    0.f,1.f,1.f, 1.f,1.f,1.f,  1.f,1.f,0.f,
    //Face 3
    0.f,0.f,0.f, 1.f,0.f,0.f, 0.f,0.f,1.f,
    1.f,0.f,0.f, 1.f,0.f,1.f, 0.f,0.f,1.f
});

const std::vector<glm::vec3> Cube::basicNormalsCube =
Utility::GLfloatListToGlmVec3({
    //Face 1 
    0.f,0.f,-1.f, 0.f,0.f,-1.f, 0.f,0.f,-1.f,
    0.f,0.f,-1.f, 0.f,0.f,-1.f, 0.f,0.f,-1.f,
    //Face 2
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    //Face 6
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
    //Face 5
    -1.f,0.f,0.f, -1.f,0.f,0.f, -1.f,0.f,0.f,
    -1.f,0.f,0.f, -1.f,0.f,0.f, -1.f,0.f,0.f,
    //Face 4
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    //Face 3
    0.f,-1.f,0.f, 0.f,-1.f,0.f, 0.f,-1.f,0.f,
    0.f,-1.f,0.f, 0.f,-1.f,0.f, 0.f,-1.f,0.f
});

const std::vector<glm::vec3> Cube::basicColorsCube =
Utility::GLfloatListToGlmVec3({

    //Face 1 
    0.5f,0.f,0.f, 0.5f,0.f,0.f, 0.5f,0.f,0.f,
    0.5f,0.f,0.f, 0.5f,0.f,0.f, 0.5f,0.f,0.f,
    //Face 2
    0.f,0.5f,0.f, 0.f,0.5f,0.f, 0.f,0.5f,0.f,
    0.f,0.5f,0.f, 0.f,0.5f,0.f, 0.f,0.5f,0.f,
    //Face 6
    0.5f,0.f,0.5f, 0.5f,0.f,0.5f, 0.5f,0.f,0.5f,
    0.5f,0.f,0.5f, 0.5f,0.f,0.5f, 0.5f,0.f,0.5f,
    //Face 5
    0.f,0.5f,0.5f, 0.f,0.5f,0.5f, 0.f,0.5f,0.5f,
    0.f,0.5f,0.5f, 0.f,0.5f,0.5f, 0.f,0.5f,0.5f,
    //Face 4
    0.5f,0.5f,0.f, 0.5f,0.5f,0.f, 0.5f,0.5f,0.f,
    0.5f,0.5f,0.f, 0.5f,0.5f,0.f, 0.5f,0.5f,0.f,
    //Face 3
    0.f,0.f,0.8f, 0.f,0.f,0.8f, 0.f,0.f,0.8f,
    0.f,0.f,0.8f, 0.f,0.f,0.8f, 0.f,0.f,0.8f
});

const std::vector<glm::vec3> Cube::iceColorsCube =
Utility::GLfloatListToGlmVec3({

    //Face 1 
    0.f,0.f,1.0f, 0.f,0.f,1.f, 0.f,0.8f,0.8f,
    0.f,0.f,1.0f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 2
    0.f,0.8f,0.8f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 6
    0.f,0.8f,0.8f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 5
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.8f,0.8f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 4
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.8f,0.8f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 3
    0.f,0.8f,0.8f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f
});

const std::vector<glm::vec3> Cube::fireColorsCube =
Utility::GLfloatListToGlmVec3({

    //Face 1
    0.8f,0.8f,0.f, 0.8f,0.8f,0.f, 1.f,0.f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 2
    1.f,0.f,0.f, 0.8f,0.8f,0.f, 0.8f,0.8f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 6
    1.f,0.f,0.f, 0.8f,0.8f,0.f, 0.8f,0.8f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 5
    0.8f,0.8f,0.f, 0.8f,0.8f,0.f, 1.f,0.f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 4
    0.8f,0.8f,0.f, 0.8f,0.8f,0.f, 1.f,0.f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 3
    1.f,0.f,0.f, 0.8f,0.8f,0.f, 0.8f,0.8f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f
});

const std::vector<glm::vec2> Cube::basicUVCoordsCube =
Utility::GLfloatListToGlmVec2({
    //Face 1 
    0.f,1.f, 1.f,0.f, 0.f,0.f,
    0.f,1.f, 1.f,1.f, 1.f,0.f,
    //Face 2
    0.f,0.f, 1.f,0.f, 0.f,1.f,
    1.f,0.f, 1.f,1.f, 0.f,1.f,
    //Face 6
    1.f,0.f, 1.f,1.f, 1.f,0.f,
    1.f,1.f, 1.f,1.f, 1.f,0.f,
    //Face 5
    0.f,0.f, 0.f,1.f, 0.f,0.f,
    0.f,0.f, 0.f,1.f, 0.f,1.f,
    //Face 4
    0.f,1.f, 1.f,1.f, 0.f,1.f,
    0.f,1.f, 1.f,1.f, 1.f,1.f,
    //Face 3
    0.f,0.f, 1.f,0.f, 0.f,0.f,
    1.f,0.f, 1.f,0.f, 0.f,0.f
});
