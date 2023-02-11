//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMVEC4_H
#define JUMPERBALLAPPLICATION_UNIFORMVEC4_H

#include "Uniform.h"

class UniformVec4;

using UniformVec4_sptr = std::shared_ptr<UniformVec4>;
using vecUniformVec4_sptr = std::vector<UniformVec4_sptr>;

class UniformVec4 : public Uniform<glm::vec4> {

public:
    UniformVec4(const glm::vec4 &value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() const override;
};

#endif //JUMPERBALLAPPLICATION_UNIFORMVEC4_H
