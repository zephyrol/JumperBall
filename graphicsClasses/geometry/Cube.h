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
    Cube(const glm::mat4& transform);
    Cube(const glm::mat4& transform,const glm::vec3& customColor);
    Cube(const Cube& cube, const glm::mat4& transform);
private:
    

    static const std::vector<glm::vec3> basicPositionsCube;
    static const std::vector<glm::vec3> basicColorsCube;
    static const std::vector<glm::vec3> basicNormalsCube;
    static const std::vector<glm::vec2> basicUVCoordsCube;
};

#endif /* CUBE_H */

