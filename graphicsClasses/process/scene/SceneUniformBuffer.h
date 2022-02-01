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
        const glm::vec1& teleportationCoeff
    );

private:
    static constexpr size_t sizeSceneUniformBuffer =
        2 * sizeof(glm::mat4) + 3 * sizeof(glm::vec3) + sizeof(glm::vec1);


    static constexpr size_t VPOffset = 0;
    static constexpr size_t VPStarOffset = sizeof(glm::mat4);
    static constexpr size_t cameraPositionOffset = VPStarOffset + sizeof(glm::mat4);
    static constexpr size_t lightDirectionOffset = cameraPositionOffset + sizeof(glm::vec3);
    static constexpr size_t flashColorOffset = lightDirectionOffset + sizeof(glm::vec3);
    static constexpr size_t teleportationCoeffOffset = flashColorOffset + sizeof(glm::vec3);

    const std::shared_ptr<glm::mat4> _VP;
    const std::shared_ptr<glm::mat4> _VPStar;
    const std::shared_ptr<glm::vec3> _cameraPosition;
    const std::shared_ptr<glm::vec3> _lightDirection;
    const std::shared_ptr<glm::vec3> _flashColor;
    const std::shared_ptr<glm::vec1> _teleportationCoeff;

    std::vector<GLubyte> _uniformBufferContent;


    struct FieldLocation {
        GLubyte* uboLocation;
        const GLfloat* fieldDataLocation;
    };

    template <typename T>
    using DataLocation = std::unordered_map<std::shared_ptr<T>, FieldLocation> ;

    template <typename T> void updateUniformBufferContent(
        const std::shared_ptr<T>& current,
        const T& newValue,
        const DataLocation<T>& dataLocation
    );

    const DataLocation<glm::mat4> _mat4DataLocations;
    const DataLocation<glm::vec3> _vec3DataLocations;
    const DataLocation<glm::vec1> _vec1DataLocations;

    DataLocation<glm::mat4> createMat4DataLocation();
    DataLocation<glm::vec3> createVec3DataLocation();
    DataLocation<glm::vec1> createVec1DataLocation();

};


template<typename T>
void SceneUniformBuffer::updateUniformBufferContent(
    const std::shared_ptr<T> &current,
    const T &newValue,
    const DataLocation<T>& dataLocation
) {
    *current = newValue;
    const auto& fieldLocation = dataLocation.at(current);
    memcpy(fieldLocation.uboLocation, fieldLocation.fieldDataLocation, sizeof(T));
}

#endif //JUMPERBALLAPPLICATION_SCENEUNIFORMBUFFER_H
