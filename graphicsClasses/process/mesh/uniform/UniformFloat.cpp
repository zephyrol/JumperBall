//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "UniformFloat.h"

UniformFloat::UniformFloat(
    GLfloat value,
    const CstShaderProgram_sptr &shaderProgram,
    const std::string &name
) : Uniform(value, shaderProgram, name) {
}

void UniformFloat::bind() const {
    glUniform1fv(_location, 1, &_value);
}
