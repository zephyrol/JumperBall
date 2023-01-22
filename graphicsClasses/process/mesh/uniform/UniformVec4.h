//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMVEC4_H
#define JUMPERBALLAPPLICATION_UNIFORMVEC4_H

#include "Uniform.h"

class UniformVec4;

using UniformVec4_uptr = std::unique_ptr<UniformVec4>;
using vecUniformVec4_uptr = std::vector<UniformVec4_uptr>;

class UniformVec4 : public Uniform<glm::vec4> {

public:
    UniformVec4(const glm::vec4 &value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() override;
};

#endif //JUMPERBALLAPPLICATION_UNIFORMVEC4_H
