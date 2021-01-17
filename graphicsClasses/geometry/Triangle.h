/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Triangle.h
 * Author: Morgenthaler S 
 *
 * Created on 16 janvier 2021, 21:23
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GeometricShape.h"

class Triangle : public GeometricShape {
public:
    //--CONSTRUCTORS & DESTRUCTORS--//
    Triangle( const glm::mat4& modelTransform    = glm::mat4(1.f),
          const glm::mat4& normalsTransform  = glm::mat4(1.f));
    Triangle( const glm::vec3& customColor,
          const glm::mat4& modelTransform    = glm::mat4(1.f),
          const glm::mat4& normalsTransform  = glm::mat4(1.f));
    Triangle( const GeometricShape& triangle,
          const glm::mat4& modelTransform    = glm::mat4(1.f),
          const glm::mat4& normalsTransform  = glm::mat4(1.f));
private:
    
    //--------STATIC METHODS-------//
    static std::vector<glm::vec3>   createCustomColors(
                                          const glm::vec3& customColor
                                          );

    static const std::vector<glm::vec3> basicPositionsTriangle;
    static const std::vector<glm::vec3> basicColorsTriangle;
    static const std::vector<glm::vec3> basicNormalsTriangle;
    static const std::vector<glm::vec2> basicUVCoordsTriangle;
};


#endif /* TRIANGLE_H */

