//
// Created by S.Morgenthaler on 22/01/2023.
//
#include "UniformVec3.h"

UniformVec3::UniformVec3(
    const glm::vec3 &value,
    const CstShaderProgram_sptr &shaderProgram,
    const std::string &name
) : Uniform(value, shaderProgram, name) {
}

void UniformVec3::bind() {
    glUniform3fv(_location, 1, &_value[0]);
}
