/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Cylinder.cpp
 * Author: Morgenthaler S
 * 
 * Created on 29 mars 2020, 09:19
 */

#include "Cylinder.h"

Cylinder::Cylinder(
                size_t meriCount,
                const glm::mat4& modelTransform,
                const glm::mat4& normalsTransform): GeometricShape( 
                              computeBasicInfoCylinder(meriCount).positions,
                              computeBasicInfoCylinder(meriCount).normals,
                              computeBasicInfoCylinder(meriCount).colors,
                              computeBasicInfoCylinder(meriCount).uvCoords,
                              modelTransform,
                              normalsTransform,
                              computeBasicInfoCylinder(meriCount).indices),
                    _meriCount (meriCount){
}

Cylinder::Cylinder(
                const glm::vec3& customColor,
                size_t meriCount,
                const glm::mat4& modelTransform,
                const glm::mat4& normalsTransform): GeometricShape ( 
                              computeBasicInfoCylinder(meriCount).positions,
                              computeBasicInfoCylinder(meriCount).normals,
                              GeometricShape::createCustomColorBuffer(
                                customColor,
                                computeBasicInfoCylinder(meriCount).colors.size()),
                              computeBasicInfoCylinder(meriCount).uvCoords,
                              modelTransform,
                              normalsTransform,
                              computeBasicInfoCylinder(meriCount).indices),
                    _meriCount (meriCount){
}

Cylinder::Cylinder( const glm::vec3& customColorCenter, 
                    const glm::vec3& customColorEdge, 
                    size_t meriCount,
                    const glm::mat4& modelTransform, 
                    const glm::mat4& normalsTransform): GeometricShape(
                              computeBasicInfoCylinder(meriCount).positions,
                              computeBasicInfoCylinder(meriCount).normals,
                              createCenterAndEdgeColorBuffer(
                                customColorCenter,
                                customColorEdge,
                                computeBasicInfoCylinder(meriCount).colors.size()),
                              computeBasicInfoCylinder(meriCount).uvCoords,
                              modelTransform,
                              normalsTransform,
                              computeBasicInfoCylinder(meriCount).indices),
                    _meriCount (meriCount){

}



Cylinder::Cylinder( const GeometricShape& cylinder,
                const glm::mat4& modelTransform,
                const glm::mat4& normalsTransform ):
    GeometricShape(cylinder,modelTransform,normalsTransform),
    _meriCount(cylinder.levelOfDetail()){
}

Cylinder::InfoCylinder Cylinder::computeBasicInfoCylinder(size_t meriCount) {

    Cylinder::InfoCylinder infoCylinder;

    constexpr float         r           = 0.5f;
   // Create a Cylinder --------------------------------------------------------
    
    const float a2 = ( 360.0f / static_cast<float>( meriCount - 1 ) ) *
        static_cast<float>(M_PI)/ 180.0f;
    
    infoCylinder.positions.push_back( glm::vec3(0.f,0.f,0.f));
    infoCylinder.normals.push_back( glm::vec3(0.f,-1.f,0.f));
    infoCylinder.colors.push_back( glm::vec3(1.f,0.f,0.f));
    infoCylinder.uvCoords.push_back( glm::vec2(0.f,0.f));
    
    infoCylinder.positions.push_back( glm::vec3(0.f,1.f,0.f));
    infoCylinder.normals.push_back( glm::vec3(0.f,1.f,0.f));
    infoCylinder.colors.push_back( glm::vec3(0.f,1.f,0.f));
    infoCylinder.uvCoords.push_back( glm::vec2(0.f,0.f));
    
    
    const glm::vec3 initialPositionBase (r,0.f,0.f);
    const glm::vec3 initialPositionTop  (r,1.f,0.f);
    infoCylinder.positions.push_back(initialPositionBase);
    infoCylinder.positions.push_back(initialPositionTop);
    infoCylinder.positions.push_back(initialPositionBase);
    infoCylinder.positions.push_back(initialPositionTop);
    
    infoCylinder.colors.push_back(glm::vec3(1.f,0.f,0.f));
    infoCylinder.colors.push_back(glm::vec3(0.f,1.f,0.f));
    infoCylinder.colors.push_back(glm::vec3(1.f,0.f,0.f));
    infoCylinder.colors.push_back(glm::vec3(0.f,1.f,0.f));
    
    infoCylinder.normals.push_back(glm::normalize(initialPositionBase));
    infoCylinder.normals.push_back(glm::normalize(initialPositionBase));
    infoCylinder.normals.push_back(glm::vec3(0.f,-1.f,0.f));
    infoCylinder.normals.push_back(glm::vec3(0.f,1.f,0.f));
    
    infoCylinder.uvCoords.push_back(
      glm::vec2( initialPositionBase.x, initialPositionBase.z));
    infoCylinder.uvCoords.push_back(
      glm::vec2( initialPositionTop.x,  initialPositionTop.z));
    infoCylinder.uvCoords.push_back(
      glm::vec2( initialPositionBase.x, initialPositionBase.z));
    infoCylinder.uvCoords.push_back(
      glm::vec2( initialPositionTop.x,  initialPositionTop.z));
    
    for( unsigned int i = 1 ; i < meriCount; ++i )
    {
        const glm::mat4 rotation = glm::rotate(a2 * i, glm::vec3(0.f,1.f,0.f));
        
        const glm::vec3 positionBase(rotation * glm::vec4(r,0.f,0.f,1.f));
        const glm::vec3 positionTop (positionBase.x,1.f,positionBase.z);

        for (unsigned int j = 0; j < 2; j++)
        {
            infoCylinder.positions.push_back(positionBase);
            infoCylinder.positions.push_back(positionTop);

            infoCylinder.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
            infoCylinder.colors.push_back(glm::vec3(0.f, 1.f, 0.f));

            infoCylinder.uvCoords.push_back(
                glm::vec2(positionBase.x, positionBase.z));
            infoCylinder.uvCoords.push_back(
                glm::vec2(positionTop.x, positionTop.z));
        }

        infoCylinder.normals.push_back(glm::normalize(positionBase));
        infoCylinder.normals.push_back(glm::normalize(positionBase));
        infoCylinder.normals.push_back(glm::vec3(0.f, -1.f, 0.f));
        infoCylinder.normals.push_back(glm::vec3(0.f, 1.f, 0.f));
    }

    for( unsigned int i = 0; i < ( meriCount - 1 ); ++i )
    {
        infoCylinder.indices.push_back(0);
        infoCylinder.indices.push_back(2+(i+1)*4+2);
        infoCylinder.indices.push_back(2+i*4+2);
        
        infoCylinder.indices.push_back(1);
        infoCylinder.indices.push_back(2+i*4+2+1);
        infoCylinder.indices.push_back(2+(i+1)*4+2+1);

        infoCylinder.indices.push_back(2+i*4);
        infoCylinder.indices.push_back(2+(i+1)*4);
        infoCylinder.indices.push_back(2+i*4+1);

        infoCylinder.indices.push_back(2+(i+1)*4);
        infoCylinder.indices.push_back(2+(i+1)*4+1);
        infoCylinder.indices.push_back(2+i*4+1);
    }
    
    return infoCylinder;
}


const Cylinder::InfoCylinder Cylinder::basicInfoCylinder = 
  computeBasicInfoCylinder();

std::vector<glm::vec3> Cylinder::createCenterAndEdgeColorBuffer(
    const glm::vec3& customColorCenter, 
    const glm::vec3& customColorEdge, 
    size_t size) {
  
    std::vector<glm::vec3> customColorCube;
    customColorCube.push_back(customColorCenter);
    customColorCube.push_back(customColorCenter);
    for(size_t i = 2; i < size ; ++i){
        customColorCube.push_back(customColorEdge);
    }
    return customColorCube;
}

size_t Cylinder::levelOfDetail() const {
    return _meriCount;
}
