/*
 * File: uniformLight.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 décembre 2019, 20:03
 */

#include <cstdio>
#include <cstring>
#include "uniformLight.h"
#include "scene/Star.h"

UniformLight::UniformLight(const glm::vec3& directionLight,
                           const glm::vec3& ambientLightIntensity,
                           const glm::vec3& diffuseLightIntensity,
                           const glm::vec3& specularLightIntensity):
    UniformBlock(
        std::move(createVariablesVecThree(
                      directionLight,
                      ambientLightIntensity,
                      diffuseLightIntensity,
                      specularLightIntensity
                      ))
        ) { }

Mesh::UniformVariables <glm::vec3> UniformLight::createVariablesVecThree (
    const glm::vec3& directionLight,
    const glm::vec3& ambientLightIntensity,
    const glm::vec3& diffuseLightIntensity,
    const glm::vec3& specularLightIntensity
    ) const {
    Mesh::UniformVariables <glm::vec3> variablesVecThree;
    variablesVecThree[Star::lightDirectionName] = directionLight;
    variablesVecThree["ambientLightIntensity"] = ambientLightIntensity;
    variablesVecThree["diffuseLightIntensity"] = diffuseLightIntensity;
    variablesVecThree["specularLightIntensity"] = specularLightIntensity;
    return variablesVecThree;
}

/*void UniformLight::directionLight (const glm::vec3& directionLight) {
    _directionLight = directionLight;
   }

   void UniformLight::ambientLightIntensity (const glm::vec3& ambLightIntensity) {
    _ambientLightIntensity = ambLightIntensity;
   }

   void UniformLight::diffuseLightIntensity (const glm::vec3& diffLightIntensity) {
    _diffuseLightIntensity = diffLightIntensity;
   }

   void UniformLight::specularLightIntensity (const glm::vec3& specLightIntensity) {
    _specularLightIntensity = specLightIntensity;
   }

   void UniformLight::update() {
    fillDataBuffer(0, _directionLight);
    fillDataBuffer(1, _ambientLightIntensity);
    fillDataBuffer(2, _diffuseLightIntensity);
    fillDataBuffer(3, _specularLightIntensity);
   }*/
