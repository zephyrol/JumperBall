/*
 * File: uniformLight.h
 * Author: Morgenthaler S
 *
 * Created on 22 décembre 2019, 20:03
 */

#ifndef UNIFORMLIGHT_H
#define UNIFORMLIGHT_H
#include "UniformBlock.h"

class UniformLight:public UniformBlock {
public:

UniformLight(const glm::vec3& directionLight,
             const glm::vec3& ambientLightIntensity,
             const glm::vec3& diffuseLightIntensity,
             const glm::vec3& specularLightIntensity);

private:

Mesh::UniformVariables <glm::vec3> createVariablesVecThree(
    const glm::vec3& directionLight,
    const glm::vec3& ambientLightIntensity,
    const glm::vec3& diffuseLightIntensity,
    const glm::vec3& specularLightIntensity
    ) const;

};
#endif /* UNIFORMLIGHT_H */
