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

UniformLight::UniformLight(const std::string& blockName,
                           const ShaderProgram& shaderProgram,
                           const glm::vec3& positionLight,
                           const glm::vec3& ambientLightIntensity,
                           const glm::vec3& diffuseLightIntensity,
                           const glm::vec3& specularLightIntensity) :
              UniformBlock (blockName, shaderProgram,
                            { "positionLight",
                              "ambientLightIntensity",
                              "diffuseLightIntensity",
                              "specularLightIntensity" }),
              _positionLight(positionLight),
              _ambientLightIntensity(ambientLightIntensity),
              _diffuseLightIntensity(diffuseLightIntensity),
              _specularLightIntensity(specularLightIntensity),
              _lightDataBuffer(createDataBuffer())
{
}

void UniformLight::positionLight(const glm::vec3& posLight) {
  _positionLight = posLight;
}

void UniformLight::ambientLightIntensity(const glm::vec3& ambLightIntensity) {
    _ambientLightIntensity = ambLightIntensity;
}

void UniformLight::diffuseLightIntensity(const glm::vec3& diffLightIntensity) {
    _diffuseLightIntensity = diffLightIntensity;
}

void UniformLight::specularLightIntensity(const glm::vec3& specLightIntensity) {
    _specularLightIntensity = specLightIntensity;
}

const std::vector<GLbyte> &UniformLight::dataBuffer() const
{
    return _lightDataBuffer;
}

void UniformLight::fillDataBuffer(std::vector<GLbyte> &dataBuffer) const
{
    memcpy      ( dataBuffer.data() + variablesOffsets().at(0),
                  &_positionLight, sizeVec3f) ;
    memcpy      ( dataBuffer.data() + variablesOffsets().at(1),
                  &_ambientLightIntensity, sizeVec3f) ;
    memcpy      ( dataBuffer.data() + variablesOffsets().at(2),
                  &_diffuseLightIntensity, sizeVec3f) ;
    memcpy      ( dataBuffer.data() + variablesOffsets().at(3),
                  &_specularLightIntensity, sizeVec3f) ;
}

std::vector<GLbyte> UniformLight::createDataBuffer() const {
    std::vector<GLbyte> lightDataBuffer(blockSize());
    fillDataBuffer(lightDataBuffer);
    return lightDataBuffer;
}

void UniformLight::update()
{
    fillDataBuffer(_lightDataBuffer);
}


void UniformLight::bind() const {

    glBindBuffer( GL_UNIFORM_BUFFER, uboHandle());
    glBufferData( GL_UNIFORM_BUFFER, blockSize(), 
                _lightDataBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboHandle());
}



