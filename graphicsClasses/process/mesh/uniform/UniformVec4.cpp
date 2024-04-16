//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "UniformVec4.h"

UniformVec4::UniformVec4(const glm::vec4& value,
                         const ShaderProgram_uptr& shaderProgram,
                         const std::string& name)
    : Uniform(value, shaderProgram, name) {}

void UniformVec4::bind() const {
    glUniform4fv(_location, 1, &_value[0]);
}
