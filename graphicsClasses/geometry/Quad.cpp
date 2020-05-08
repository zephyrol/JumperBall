/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Quad.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 30 mars 2020, 18:34
 */

#include "Quad.h"

Quad::Quad( const glm::mat4& modelTransform,
            const glm::mat4& normalsTransform ): 
    GeometricShape ( 
        basicPositionsQuad,
        basicNormalsQuad,
        basicColorsQuad,
        basicUVCoordsQuad,
        modelTransform,
        normalsTransform) {
}

Quad::Quad( const glm::vec3& customColor,
            const glm::mat4& modelTransform,
            const glm::mat4& normalsTransform ): 
    GeometricShape (
        basicPositionsQuad,
        basicNormalsQuad,
        GeometricShape::createCustomColorBuffer(customColor, 
        basicPositionsQuad.size()),
        basicUVCoordsQuad,
        modelTransform,
        normalsTransform)
{
}


Quad::Quad(const GeometricShape& quad, 
           const glm::mat4& modelTransform,
           const glm::mat4& normalsTransform ):
    GeometricShape(quad,modelTransform,normalsTransform) {
}


const std::vector<glm::vec3> Quad::basicPositionsQuad =
Utility::GLfloatListToGlmVec3({
    -1.f,1.f,0.f, -1.f,-1.f,0.f,  1.f,-1.f,0.f,  
    -1.f,1.f,0.f,  1.f,-1.f,0.f,   1.f,1.f,0.f
});

const std::vector<glm::vec3> Quad::basicNormalsQuad =
Utility::GLfloatListToGlmVec3({
    0.f,0.f,-1.f, 0.f,0.f,-1.f,  0.f,0.f,-1.f,
    0.f,0.f,-1.f, 0.f,0.f,-1.f,  0.f,0.f,-1.f
});

const std::vector<glm::vec3> Quad::basicColorsQuad =
Utility::GLfloatListToGlmVec3({
    1.f,1.f,1.f, 1.f,1.f,1.f,  1.f,1.f,1.f,
    1.f,1.f,1.f, 1.f,1.f,1.f,  1.f,1.f,1.f
});

const std::vector<glm::vec2> Quad::basicUVCoordsQuad =
Utility::GLfloatListToGlmVec2({
    0.f,1.f, 0.f,0.f, 1.f,0.f,  
    0.f,1.f, 1.f,0.f, 1.f,1.f
});