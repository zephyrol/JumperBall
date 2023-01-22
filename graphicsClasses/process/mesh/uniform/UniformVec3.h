//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMVEC3_H
#define JUMPERBALLAPPLICATION_UNIFORMVEC3_H


#include "Uniform.h"

class UniformVec3;

using UniformVec3_uptr = std::unique_ptr<UniformVec3>;
using vecUniformVec3_uptr = std::vector<UniformVec3_uptr>;

class UniformVec3 : public Uniform<glm::vec3> {
public:
    UniformVec3(const glm::vec3 &value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() override;
};

#endif //JUMPERBALLAPPLICATION_UNIFORMVEC3_H
