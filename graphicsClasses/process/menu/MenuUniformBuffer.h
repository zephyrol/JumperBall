//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_MENUUNIFORMBUFFER_H
#define JUMPERBALLAPPLICATION_MENUUNIFORMBUFFER_H


#include "process/UniformBuffer.h"

class MenuUniformBuffer: public UniformBuffer {
public:

    MenuUniformBuffer(
        const vecCstShaderProgram_sptr& menuShaderPrograms
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
    static constexpr size_t sizeMenuUniformBuffer =
        2 * sizeof(glm::mat4) + 3 * sizeof (glm::vec3) + sizeof (float);

    static UniformBuffer::ShaderProgramBindingPoint getBindingPointMap(
        const vecCstShaderProgram_sptr& menuShaderPrograms
    );

    glm::mat4 _VP;
    glm::mat4 _VPStar;
    glm::vec3 _cameraPosition;
    glm::vec3 _lightDirection;
    glm::vec3 _flashColor;
    float _teleportationCoeff;
};


#endif //JUMPERBALLAPPLICATION_MENUUNIFORMBUFFER_H
