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
UniformLight(const std::string& blockName,
             const ShaderProgram& shaderProgram,
             const glm::vec3& directionLight,
             const glm::vec3& ambientLightIntensity,
             const glm::vec3& diffuseLightIntensity,
             const glm::vec3& specularLightIntensity);
~UniformLight() override = default;

void bind() const override;

void update() override;
void directionLight(const glm::vec3& directionLight);
void ambientLightIntensity(const glm::vec3& ambLightIntensity);
void diffuseLightIntensity(const glm::vec3& diffLightIntensity);
void specularLightIntensity(const glm::vec3& specLightIntensity);

private:

virtual const std::vector <GLbyte>& dataBuffer() const override;
virtual std::vector <GLbyte> createDataBuffer() const override;
virtual void fillDataBuffer(std::vector <GLbyte>& dataBuffer) const override;

glm::vec3 _directionLight;
glm::vec3 _ambientLightIntensity;
glm::vec3 _diffuseLightIntensity;
glm::vec3 _specularLightIntensity;
std::vector <GLbyte> _lightDataBuffer;
};

#endif /* UNIFORMLIGHT_H */
