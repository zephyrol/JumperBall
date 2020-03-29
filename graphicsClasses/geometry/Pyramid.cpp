/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pyramid.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 29 mars 2020, 09:19
 */

#include "Pyramid.h"


Pyramid::Pyramid(const glm::mat4& transform): GeometricShape (
                                                        basicPositionsPyramid,
                                                        basicNormalsPyramid,
                                                        basicColorsPyramid,
                                                        basicUVCoordsPyramid,
                                                        transform) {
}

Pyramid::Pyramid(const glm::mat4& transform,const glm::vec3& customColor): 
    GeometricShape (
        basicPositionsPyramid,
        basicNormalsPyramid,
        GeometricShape::createCustomColorBuffer(customColor, 
        basicPositionsPyramid.size()),
        basicUVCoordsPyramid,
        transform)
{
}


Pyramid::Pyramid(const Pyramid& pyramid, const glm::mat4& transform) :
    GeometricShape(pyramid,transform) {
}


const std::vector<glm::vec3> Pyramid::basicPositionsPyramid =
Utility::GLfloatListToGlmVec3({

    //Base
    0.f,0.f,1.f,    0.f,0.f,0.f,    1.f,0.f,0.f, 
    0.f,0.f,1.f,    1.f,0.f,0.f,    1.f,0.f,1.f, 
    //Face 1
    0.f,0.f,0.f,    0.5f,1.f,0.5f,  0.5f,0.f,0.f, 
    0.5f,0.f,0.f,   0.5f,1.f,0.5f,  1.f,0.f,0.f, 
    //Face 2
    0.5f,0.f,1.f,   0.5f,1.f,0.5f,  0.f,0.f,1.f, 
    1.f,0.f,1.f,    0.5f,1.f,0.5f,  0.5f,0.f,1.f,
    //Face 3
    0.f,0.f,0.f,    0.f,0.f,0.5f,   0.5f,1.f,0.5f,
    0.f,0.f,0.5f,   0.f,0.f,1.f,    0.5f,1.f,0.5f,
    //Face 4
    1.f,0.f,0.5f,   1.f,0.f,0.f,    0.5f,1.f,0.5f,
    1.f,0.f,1.f,    1.f,0.f,0.5f,   0.5f,1.f,0.5f
});

const std::vector<glm::vec3> Pyramid::basicNormalsPyramid =
Utility::GLfloatListToGlmVec3( Utility::computeNormals({
    //Base
    0.f,0.f,1.f,    0.f,0.f,0.f,    1.f,0.f,0.f, 
    0.f,0.f,1.f,    1.f,0.f,0.f,    1.f,0.f,1.f, 
    //Face 1
    0.f,0.f,0.f,    0.5f,1.f,0.5f,  0.5f,0.f,0.f, 
    0.5f,0.f,0.f,   0.5f,1.f,0.5f,  1.f,0.f,0.f, 
    //Face 2
    0.5f,0.f,1.f,   0.5f,1.f,0.5f,  0.f,0.f,1.f, 
    1.f,0.f,1.f,    0.5f,1.f,0.5f,  0.5f,0.f,1.f,
    //Face 3
    0.f,0.f,0.f,    0.f,0.f,0.5f,   0.5f,1.f,0.5f,
    0.f,0.f,0.5f,   0.f,0.f,1.f,    0.5f,1.f,0.5f,
    //Face 4
    1.f,0.f,0.5f,   1.f,0.f,0.f,    0.5f,1.f,0.5f,
    1.f,0.f,1.f,    1.f,0.f,0.5f,   0.5f,1.f,0.5f
}));

const std::vector<glm::vec3> Pyramid::basicColorsPyramid =
Utility::GLfloatListToGlmVec3({
    //Base
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    //Face 1
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.1f,0.1f,0.1f, 
    //Face 2
    0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  0.1f,0.1f,0.1f, 
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f,  0.5f,0.5f,0.5f, 
    //Face 3
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,  
    0.5f,0.5f,0.5f, 0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f, 
    //Face 4
    0.5f,0.5f,0.5f, 0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f,
    0.1f,0.1f,0.1f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,
});

const std::vector<glm::vec2> Pyramid::basicUVCoordsPyramid =
Utility::GLfloatListToGlmVec2({
    //Base
    0.f,0.f,    0.f,0.f,   1.f,0.f, 
    0.f,0.f,    1.f,0.f,   1.f,0.f, 
    //Face 1
    0.f,0.f,    0.5f,1.f,  0.5f,0.f, 
    0.5f,0.f,   0.5f,1.f,  1.f,0.f, 
    //Face 2
    0.5f,0.f,   0.5f,1.f,  0.f,0.f, 
    1.f,0.f,    0.5f,1.f,  0.5f,0.f,
    //Face 3
    0.f,0.f,    0.f,0.f,   0.5f,1.f,
    0.f,0.f,    0.f,0.f,   0.5f,1.f,
    //Face 4
    1.f,0.f,    1.f,0.f,   0.5f,1.f,
    1.f,0.f,    1.f,0.f,   0.5f,1.f
});

