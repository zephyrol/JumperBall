/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Pyramid.h
 * Author: Morgenthaler S 
 *
 * Created on 29 mars 2020, 09:19
 */

#ifndef PYRAMID_H
#define PYRAMID_H

#include "GeometricShape.h"

class Pyramid: public GeometricShape {
public:
    Pyramid(const glm::mat4& transform);
    Pyramid(const glm::mat4& transform,const glm::vec3& customColor);
    Pyramid(const Pyramid& pyramid, const glm::mat4& transform);
private:
    

    static const std::vector<glm::vec3> basicPositionsPyramid;
    static const std::vector<glm::vec3> basicColorsPyramid;
    static const std::vector<glm::vec3> basicNormalsPyramid;
    static const std::vector<glm::vec2> basicUVCoordsPyramid;
};
#endif /* PYRAMID_H */

