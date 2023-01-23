//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMS_H
#define JUMPERBALLAPPLICATION_UNIFORMS_H


#include "UniformVec4.h"
#include "UniformVec3.h"
#include "UniformVec2.h"
#include "UniformFloat.h"
#include "UniformInt.h"
#include "scene/Displayable.h"

class Uniforms {

public:

    explicit Uniforms(
        vecUniformVec4_sptr uniformsVec4,
        vecUniformVec3_sptr uniformsVec3,
        vecUniformVec2_sptr uniformsVec2,
        vecUniformFloat_sptr uniformsFloat,
        vecUniformInt_sptr uniformsInt
    );

    void bind() const;
    void update(
        const std::vector<glm::vec4>& vec4s,
        const std::vector<glm::vec3>& vec3s,
        const std::vector<glm::vec2>& vec2s,
        const std::vector<GLfloat>& floats,
        const std::vector<GLint>& ints
    );

private:

    template <typename T>
    void updateUniforms(
        const std::vector<T>& newValues,
        vecUniform_sptr<T>& currentUniforms
    );

    const vecUniformVec4_sptr _uniformsVec4;
    const vecUniformVec3_sptr _uniformsVec3;
    const vecUniformVec2_sptr _uniformsVec2;
    const vecUniformFloat_sptr _uniformsFloat;
    const vecUniformInt_sptr _uniformsInt;
    vecUniformBase_sptr _uniformsToBind;
};

template<typename T>
void Uniforms::updateUniforms(const std::vector<T> &newValues, vecUniform_sptr<T> &currentUniforms) {

    for(size_t i = 0; i < _uniformsVec4.size(); ++i) {
        auto& currentUniform = currentUniforms[i];
        auto& newValue = newValues[i];
        if(newValue != currentUniform->getValue()) {
            currentUniform->update(newValue);
            _uniformsToBind.push_back(currentUniform);
        }
    }
}


#endif //JUMPERBALLAPPLICATION_UNIFORMS_H
