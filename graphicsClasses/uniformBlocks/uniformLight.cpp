/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uniformLight.cpp
 * Author: morgenthaler
 * 
 * Created on 22 décembre 2019, 20:03
 */

#include <cstdio>
#include <cstring>
#include "uniformLight.h"

UniformLight::UniformLight (  //const ShaderProgram& sp, 
                              //const glm::vec3 positionLight,
                              //const glm::vec3 ambiantLightIntensity,
                              //const glm::vec3 diffuseLightIntensity,
                              //const glm::vec3 specularLightIntensity
                            ):
              UniformBlock (//sp,
                            //name,
                            { "positionLight","ambiantLightIntensity",
                              "diffuseLightIntensity","specularLightIntensity"
                            }
                            ),
              _positionLight(),
              _ambiantLightIntensity(),
              _diffuseLightIntensity(),
              _specularLightIntensity()
{
  /*constexpr size_t sizeVec3f = 3 * sizeof(GLfloat);
  memcpy      ( _dataInsideBlock.data() + _variablesOffset.at(0), 
                &positionLight, sizeVec3f) ;
  memcpy      ( _dataInsideBlock.data() + _variablesOffset.at(1), 
                &ambiantLightIntensity, sizeVec3f) ;
  memcpy      ( _dataInsideBlock.data() + _variablesOffset.at(2), 
                &diffuseLightIntensity, sizeVec3f) ;
  memcpy      ( _dataInsideBlock.data() + _variablesOffset.at(3), 
                &specularLightIntensity, sizeVec3f) ;
  
  glBindBuffer( GL_UNIFORM_BUFFER, _uboHandle);
  glBufferData( GL_UNIFORM_BUFFER, _blockSize, 
                _dataInsideBlock.data(), GL_DYNAMIC_DRAW);*/
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

    std::pair<GLint, std::vector<GLint> > infoDataBuffer = 
                                                  configureDataBuffer(sp,name);
    std::vector<GLchar> dataBuffer (infoDataBuffer.first);

    memcpy      ( dataBuffer.data() + infoDataBuffer.second.at(0), 
                  &_positionLight, sizeVec3f) ;
    memcpy      ( dataBuffer.data() + infoDataBuffer.second.at(1), 
                  &_ambiantLightIntensity, sizeVec3f) ;
    memcpy      ( dataBuffer.data() + infoDataBuffer.second.at(2), 
                  &_diffuseLightIntensity, sizeVec3f) ;
    memcpy      ( dataBuffer.data() + infoDataBuffer.second.at(3), 
                  &_specularLightIntensity, sizeVec3f) ;
  
    glBindBuffer( GL_UNIFORM_BUFFER, _uboHandle);
    glBufferData( GL_UNIFORM_BUFFER, infoDataBuffer.first, 
                dataBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, _uboHandle);
    
}






