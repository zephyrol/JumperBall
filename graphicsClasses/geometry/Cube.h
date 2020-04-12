/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cube.h
 * Author: Morgenthaler S 
 *
 * Created on 29 mars 2020, 09:18
 */

#ifndef CUBE_H
#define CUBE_H
#include "GeometricShape.h"

class Cube : public GeometricShape {

public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Cube( const glm::mat4& modelTransform    = glm::mat4(1.f),
          const glm::mat4& normalsTransform  = glm::mat4(1.f));
    Cube( const glm::vec3& customColor,
          const glm::mat4& modelTransform    = glm::mat4(1.f),
          const glm::mat4& normalsTransform  = glm::mat4(1.f));
    Cube( const GeometricShape& cube, 
          const glm::mat4& modelTransform    = glm::mat4(1.f),
          const glm::mat4& normalsTransform  = glm::mat4(1.f));


private:

    static const std::vector<glm::vec3> basicPositionsCube;
    static const std::vector<glm::vec3> basicColorsCube;
    static const std::vector<glm::vec3> basicNormalsCube;
    static const std::vector<glm::vec2> basicUVCoordsCube;
};

#endif /* CUBE_H */

