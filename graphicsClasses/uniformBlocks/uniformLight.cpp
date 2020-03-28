/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uniformLight.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 22 décembre 2019, 20:03
 */

#include <cstdio>
#include <cstring>
#include "uniformLight.h"

UniformLight::UniformLight ():
              UniformBlock ( { "positionLight","ambiantLightIntensity",
                              "diffuseLightIntensity","specularLightIntensity" }
                            ),
              _positionLight(),
              _ambiantLightIntensity(),
              _diffuseLightIntensity(),
              _specularLightIntensity()
{
}

UniformLight::UniformLight( const glm::vec3& positionLight, 
                            const glm::vec3& ambiantLightIntensity, 
                            const glm::vec3& diffuseLightIntensity, 
                            const glm::vec3& specularLightIntensity) :
              UniformBlock ( { "positionLight","ambiantLightIntensity",
                              "diffuseLightIntensity","specularLightIntensity" }
                            ),
              _positionLight(positionLight),
              _ambiantLightIntensity(ambiantLightIntensity),
              _diffuseLightIntensity(diffuseLightIntensity),
              _specularLightIntensity(specularLightIntensity) {

}


void UniformLight::positionLight(const glm::vec3& posLight) {
  _positionLight = posLight;
}

void UniformLight::ambiantLightIntensity(const glm::vec3& ambLightIntensity) {
    _ambiantLightIntensity = ambLightIntensity;
}

void UniformLight::diffuseLightIntensity(const glm::vec3& diffLightIntensity) {
    _diffuseLightIntensity = diffLightIntensity;
}

void UniformLight::specularLightIntensity(const glm::vec3& specLightIntensity) {
    _specularLightIntensity = specLightIntensity;
}

void UniformLight::bind(const std::string& name,const ShaderProgram& sp) {

    configureDataBuffer(sp,name);

    memcpy      ( _dataBuffer.data() + variablesOffsets().at(0), 
                  &_positionLight, sizeVec3f) ;
    memcpy      ( _dataBuffer.data() + variablesOffsets().at(1), 
                  &_ambiantLightIntensity, sizeVec3f) ;
    memcpy      ( _dataBuffer.data() + variablesOffsets().at(2), 
                  &_diffuseLightIntensity, sizeVec3f) ;
    memcpy      ( _dataBuffer.data() + variablesOffsets().at(3), 
                  &_specularLightIntensity, sizeVec3f) ;
  
    glBindBuffer( GL_UNIFORM_BUFFER, uboHandle());
    glBufferData( GL_UNIFORM_BUFFER, blockSize(), 
                _dataBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboHandle());
    
}






