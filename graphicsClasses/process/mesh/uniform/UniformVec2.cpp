//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "UniformVec2.h"

UniformVec2::UniformVec2(
    const glm::vec2 &value,
    const CstShaderProgram_sptr &shaderProgram,
    const std::string &name
) : Uniform(value, shaderProgram, name) {
}

void UniformVec2::bind() const {
    glUniform2fv(_location, 1, &_value[0]);
}
