//
// Created by S.Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_SCENEUNIFORMBUFFER_H
#define JUMPERBALLAPPLICATION_SCENEUNIFORMBUFFER_H


#include "process/UniformBufferBase.h"

class SceneUniformBuffer {
public:

    explicit SceneUniformBuffer(
        const vecCstShaderProgram_sptr &sceneShaderPrograms
    );

    void update(
        const glm::mat4 &VP,
        const glm::mat4 &VPStar,
        const glm::mat4 &VPStar2,
        const glm::vec3 &cameraPosition,
        const glm::vec3 &lightDirection,
        const glm::vec3 &light2Direction,
        const glm::vec3 &flashColor,
        const glm::vec1 &teleportationCoeff
    );

private:
    std::unique_ptr<UniformBufferBase> _uniformBufferBase;
};


#endif //JUMPERBALLAPPLICATION_SCENEUNIFORMBUFFER_H
