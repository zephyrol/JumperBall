/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sphere.h
 * Author: Morgenthaler S
 *
 * Created on 29 mars 2020, 14:43
 */

#ifndef SPHERE_H
#define SPHERE_H
#include "GeometricShape.h"


class Sphere : GeometricShape {
public:
    Sphere(const glm::mat4& transform);
    Sphere(const glm::mat4& transform,const glm::vec3& customColor);
    Sphere(const Sphere& sphere, const glm::mat4& transform);

    struct InfoSphere {
      std::vector<glm::vec3> positions;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec3> colors;
      std::vector<glm::vec2> uvCoords;
      std::vector<GLushort>  indices;
    };

private:

    static const InfoSphere basicInfoSphere;

    static InfoSphere computeBasicInfoSphere();

};

#endif /* SPHERE_H */

