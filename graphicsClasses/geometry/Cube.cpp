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

Cube::Cube(const glm::mat4& transform): GeometricShape( basicPositionsCube,
                                                        basicNormalsCube,
                                                        basicColorsCube,
                                                        basicUVCoordsCube,
                                                        transform) {
}

Cube::Cube(const glm::mat4& transform,const glm::vec3& customColor): 
    GeometricShape (
        basicPositionsCube,
        basicNormalsCube,
        GeometricShape::createCustomColorBuffer(customColor, 
        basicPositionsCube.size()),
        basicUVCoordsCube,
        transform)
{
}


Cube::Cube(const Cube& cube, const glm::mat4& transform) :
    GeometricShape(cube,transform) {
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