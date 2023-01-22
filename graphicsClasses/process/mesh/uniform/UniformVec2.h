//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMVEC2_H
#define JUMPERBALLAPPLICATION_UNIFORMVEC2_H


#include "Uniform.h"

class UniformVec2;

using UniformVec2_uptr = std::unique_ptr<UniformVec2>;
using vecUniformVec2_uptr = std::vector<UniformVec2_uptr>;

class UniformVec2 : public Uniform<glm::vec2> {
public:
    UniformVec2(const glm::vec2 &value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() override;
};

#endif //JUMPERBALLAPPLICATION_UNIFORMVEC2_H
