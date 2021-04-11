/*
 * File: uniformLight.h
 * Author: Morgenthaler S
 *
 * Created on 22 décembre 2019, 20:03
 */

#ifndef UNIFORMLIGHT_H
#define UNIFORMLIGHT_H
#include "uniformBlock.h"

class UniformLight:public UniformBlock {
public:

UniformLight(const glm::vec3& directionLight,
             const glm::vec3& ambientLightIntensity,
             const glm::vec3& diffuseLightIntensity,
             const glm::vec3& specularLightIntensity);

/*void update() override;

   void directionLight(const glm::vec3& directionLight);
   void ambientLightIntensity(const glm::vec3& ambLightIntensity);
   void diffuseLightIntensity(const glm::vec3& diffLightIntensity);
   void specularLightIntensity(const glm::vec3& specLightIntensity);*/


private:
/* glm::vec3 _directionLight;
   glm::vec3 _ambientLightIntensity;
   glm::vec3 _diffuseLightIntensity;
   glm::vec3 _specularLightIntensity;*/

Mesh::UniformVariables <glm::vec3> createVariablesVecThree(
    const glm::vec3& directionLight,
    const glm::vec3& ambientLightIntensity,
    const glm::vec3& diffuseLightIntensity,
    const glm::vec3& specularLightIntensity
    ) const;

};
#endif /* UNIFORMLIGHT_H */
