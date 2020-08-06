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
            const std::array<bool,6>& sides
        ): 
    GeometricShape ( 
        basicPositionsCube,
        basicNormalsCube,
        basicColorsCube,
        basicUVCoordsCube,
        modelTransform,
        normalsTransform) {
}

Cube::Cube( const glm::vec3& customColor,
            const glm::mat4& modelTransform,
            const glm::mat4& normalsTransform,
            const std::array<bool,6>& sides
        ): 
    GeometricShape (
        basicPositionsCube,
        basicNormalsCube,
        GeometricShape::createCustomColorBuffer(customColor, 
        basicPositionsCube.size()),
        basicUVCoordsCube,
        modelTransform,
        normalsTransform)
{
}


Cube::Cube( const std::vector<glm::vec3>& customColors,
            const std::array<bool,6>& sides,
            const glm::mat4& modelTransform,
            const glm::mat4& normalsTransform
        ):
    GeometricShape (
        basicPositionsCube,
        basicNormalsCube,
        customColors,
        basicUVCoordsCube,
        modelTransform,
        normalsTransform)
{
}


Cube::Cube(const GeometricShape& cube,
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform,
           const std::array<bool,6>& sides
           ):
    GeometricShape(cube,modelTransform,normalsTransform) {
    }

const std::vector<glm::vec3> Cube::getBasicPosCube(const std::array<bool, 6> &sides) {
    std::vector<glm::vec3> positions {};
    if (sides.at(0))
        positions.insert(positions.end(), basicPositionsCube.begin(),
                         basicPositionsCube.begin() + 18);
    if (sides.at(1))
        positions.insert(positions.end(), basicPositionsCube.begin() + 18,
                         basicPositionsCube.begin() + 18*2);
    if (sides.at(2))
        positions.insert(positions.end(), basicPositionsCube.begin() + 18*5,
                         basicPositionsCube.begin() + 18*6);
    if (sides.at(3))
        positions.insert(positions.end(), basicPositionsCube.begin() + 18*4,
                         basicPositionsCube.begin() + 18*5);
    if (sides.at(4))
        positions.insert(positions.end(), basicPositionsCube.begin() + 18*3,
                         basicPositionsCube.begin() + 18*4);
    if (sides.at(5))
        positions.insert(positions.end(), basicPositionsCube.begin() + 18*2,
                         basicPositionsCube.begin() + 18*3);
    return positions;
}



const std::vector<glm::vec3> Cube::basicPositionsCube =
Utility::GLfloatListToGlmVec3({
    //Face 1 
    0.f,1.f,0.f, 1.f,0.f,0.f,  0.f,0.f,0.f,
    0.f,1.f,0.f, 1.f,1.f,0.f,  1.f,0.f,0.f,
    //Face 2
    0.f,0.f,1.f, 1.f,0.f,1.f, 0.f,1.f,1.f,
    1.f,0.f,1.f, 1.f,1.f,1.f, 0.f,1.f,1.f,
    //Face 3
    0.f,0.f,0.f, 1.f,0.f,0.f, 0.f,0.f,1.f,
    1.f,0.f,0.f, 1.f,0.f,1.f, 0.f,0.f,1.f,
    //Face 4
    0.f,1.f,1.f, 1.f,1.f,0.f,  0.f,1.f,0.f, 
    0.f,1.f,1.f, 1.f,1.f,1.f,  1.f,1.f,0.f, 
    //Face 5
    0.f,0.f,1.f, 0.f,1.f,0.f,  0.f,0.f,0.f, 
    0.f,0.f,1.f, 0.f,1.f,1.f,  0.f,1.f,0.f, 
    //Face 6
    1.f,0.f,0.f, 1.f,1.f,0.f, 1.f,0.f,1.f,
    1.f,1.f,0.f, 1.f,1.f,1.f, 1.f,0.f,1.f
});

const std::vector<glm::vec3> Cube::basicNormalsCube =
Utility::GLfloatListToGlmVec3({
    //Face 1 
    0.f,0.f,-1.f, 0.f,0.f,-1.f, 0.f,0.f,-1.f,
    0.f,0.f,-1.f, 0.f,0.f,-1.f, 0.f,0.f,-1.f,
    //Face 2
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    //Face 3
    0.f,-1.f,0.f, 0.f,-1.f,0.f, 0.f,-1.f,0.f,
    0.f,-1.f,0.f, 0.f,-1.f,0.f, 0.f,-1.f,0.f,
    //Face 4 
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    0.f,1.f,0.f, 0.f,1.f,0.f, 0.f,1.f,0.f,
    //Face 5
    -1.f,0.f,0.f, -1.f,0.f,0.f, -1.f,0.f,0.f,
    -1.f,0.f,0.f, -1.f,0.f,0.f, -1.f,0.f,0.f,
    //Face 6
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
    1.f,0.f,0.f, 1.f,0.f,0.f, 1.f,0.f,0.f,
});

const std::vector<glm::vec3> Cube::basicColorsCube =
Utility::GLfloatListToGlmVec3({

    //Face 1 
    0.5f,0.f,0.f, 0.5f,0.f,0.f, 0.5f,0.f,0.f,
    0.5f,0.f,0.f, 0.5f,0.f,0.f, 0.5f,0.f,0.f,
    //Face 2
    0.f,0.5f,0.f, 0.f,0.5f,0.f, 0.f,0.5f,0.f,
    0.f,0.5f,0.f, 0.f,0.5f,0.f, 0.f,0.5f,0.f,
    //Face 3
    0.f,0.f,0.8f, 0.f,0.f,0.8f, 0.f,0.f,0.8f,
    0.f,0.f,0.8f, 0.f,0.f,0.8f, 0.f,0.f,0.8f,
    //Face 4 
    0.5f,0.5f,0.f, 0.5f,0.5f,0.f, 0.5f,0.5f,0.f,
    0.5f,0.5f,0.f, 0.5f,0.5f,0.f, 0.5f,0.5f,0.f,
    //Face 5
    0.f,0.5f,0.5f, 0.f,0.5f,0.5f, 0.f,0.5f,0.5f,
    0.f,0.5f,0.5f, 0.f,0.5f,0.5f, 0.f,0.5f,0.5f,
    //Face 6
    0.5f,0.f,0.5f, 0.5f,0.f,0.5f, 0.5f,0.f,0.5f,
    0.5f,0.f,0.5f, 0.5f,0.f,0.5f, 0.5f,0.f,0.5f
});

const std::vector<glm::vec3> Cube::iceColorsCube =
Utility::GLfloatListToGlmVec3({

    //Face 1 
    0.f,0.f,1.0f, 0.f,0.f,1.f, 0.f,0.8f,0.8f,
    0.f,0.f,1.0f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 2
    0.f,0.8f,0.8f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 3
    0.f,0.8f,0.8f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 4 
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.8f,0.8f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 5
    0.f,0.f,1.f, 0.f,0.f,1.f, 0.f,0.8f,0.8f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
    //Face 6
    0.f,0.8f,0.8f, 0.f,0.f,1.f, 0.f,0.f,1.f,
    0.f,0.f,1.f, 0.f,0.8f,0.8f, 0.f,0.f,1.f,
});

const std::vector<glm::vec3> Cube::fireColorsCube =
Utility::GLfloatListToGlmVec3({

    //Face 1
    0.8f,0.8f,0.f, 0.8f,0.8f,0.f, 1.f,0.f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 2
    1.f,0.f,0.f, 0.8f,0.8f,0.f, 0.8f,0.8f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 3
    1.f,0.f,0.f, 0.8f,0.8f,0.f, 0.8f,0.8f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 4
    0.8f,0.8f,0.f, 0.8f,0.8f,0.f, 1.f,0.f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 5
    0.8f,0.8f,0.f, 0.8f,0.8f,0.f, 1.f,0.f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
    //Face 6
    1.f,0.f,0.f, 0.8f,0.8f,0.f, 0.8f,0.8f,0.f,
    0.8f,0.8f,0.f, 1.f,0.f,0.f, 0.8f,0.8f,0.f,
});

const std::vector<glm::vec2> Cube::basicUVCoordsCube =
Utility::GLfloatListToGlmVec2({
    //Face 1 
    0.f,1.f, 1.f,0.f, 0.f,0.f,
    0.f,1.f, 1.f,1.f, 1.f,0.f,
    //Face 2
    0.f,0.f, 1.f,0.f, 0.f,1.f,
    1.f,0.f, 1.f,1.f, 0.f,1.f,
    //Face 3
    0.f,0.f, 1.f,0.f, 0.f,0.f,
    1.f,0.f, 1.f,0.f, 0.f,0.f,
    //Face 4
    0.f,1.f, 1.f,1.f, 0.f,1.f, 
    0.f,1.f, 1.f,1.f, 1.f,1.f, 
    //Face 5
    0.f,0.f, 0.f,1.f, 0.f,0.f, 
    0.f,0.f, 0.f,1.f, 0.f,1.f, 
    //Face 6
    1.f,0.f, 1.f,1.f, 1.f,0.f,
    1.f,1.f, 1.f,1.f, 1.f,0.f
});
