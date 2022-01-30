//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_SCENEUNIFORMBUFFER_H
#define JUMPERBALLAPPLICATION_SCENEUNIFORMBUFFER_H


#include "process/UniformBuffer.h"

class SceneUniformBuffer: public UniformBuffer {
public:

    SceneUniformBuffer(
        const vecCstShaderProgram_sptr& sceneShaderPrograms
    );

    void update(
        const glm::mat4& VP,
        const glm::mat4& VPStar,
        const glm::vec3& cameraPosition,
        const glm::vec3& lightDirection,
        const glm::vec3& flashColor,
        float teleportationCoeff
    );

private:
    static constexpr size_t sizeSceneUniformBuffer = 2 * sizeof(glm::mat4) +
                                                     3 * sizeof (glm::vec3) + sizeof (float);

    static UniformBuffer::ShaderProgramBindingPoint getBindingPointMap(
        const vecCstShaderProgram_sptr& sceneShaderPrograms
    );

    glm::mat4 _VP;
    glm::mat4 _VPStar;
    glm::vec3 _cameraPosition;
    glm::vec3 _lightDirection;
    glm::vec3 _flashColor;
    float _teleportationCoeff;
};


#endif //JUMPERBALLAPPLICATION_SCENEUNIFORMBUFFER_H
