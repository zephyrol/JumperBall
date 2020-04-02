/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cylinder.h
 * Author: Morgenthaler S 
 *
 * Created on 29 mars 2020, 09:19
 */

#ifndef CYLINDER_H
#define CYLINDER_H
#include "GeometricShape.h"

class Cylinder: public GeometricShape {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Cylinder( const glm::mat4& modelTransform    = glm::mat4(1.f),
              const glm::mat4& normalsTransform  = glm::mat4(1.f));
    Cylinder( const glm::vec3& customColor,
              const glm::mat4& modelTransform    = glm::mat4(1.f),
              const glm::mat4& normalsTransform  = glm::mat4(1.f));
    Cylinder( const Cylinder& cylinder, 
              const glm::mat4& modelTransform    = glm::mat4(1.f),
              const glm::mat4& normalsTransform  = glm::mat4(1.f));


    //------------TYPES------------//
    struct InfoCylinder{
      InfoCylinder():positions{},normals{},colors{},uvCoords{},indices{}{}
      std::vector<glm::vec3> positions;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec3> colors;
      std::vector<glm::vec2> uvCoords;
      std::vector<GLushort>  indices;
      ~InfoCylinder() {}
    };

private:

    static const InfoCylinder basicInfoCylinder;

    //--------STATIC METHODS-------//
    static InfoCylinder computeBasicInfoCylinder();

};

#endif /* CYLINDER_H */

