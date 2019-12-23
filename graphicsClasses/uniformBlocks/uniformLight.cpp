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

#include "uniformLight.h"
#include <cstdio>
#include <cstring>

UniformLight::UniformLight (  const ShaderProgram& sp, 
                              const std::string name,
                              const glm::vec3 positionLight,
                              const glm::vec3 ambiantLightIntensity,
                              const glm::vec3 diffuseLightIntensity,
                              const glm::vec3 specularLightIntensity
                            ):
              UniformBlock (sp,
                            name,
                            { "positionLight","ambiantLightIntensity",
                              "diffuseLightIntensity","specularLightIntensity",
                            }
                            )
{
  constexpr size_t sizeVec3f = 3 * sizeof(GLfloat);
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
                _dataInsideBlock.data(), GL_DYNAMIC_DRAW);
}


