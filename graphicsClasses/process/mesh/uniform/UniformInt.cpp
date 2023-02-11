//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "UniformInt.h"

UniformInt::UniformInt(
    GLint value,
    const CstShaderProgram_sptr &shaderProgram,
    const std::string &name
) : Uniform(value, shaderProgram, name) {
}

void UniformInt::bind() const {
    glUniform1iv(_location, 1, &_value);
}
