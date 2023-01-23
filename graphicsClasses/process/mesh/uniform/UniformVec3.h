//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMVEC3_H
#define JUMPERBALLAPPLICATION_UNIFORMVEC3_H


#include "Uniform.h"

class UniformVec3;

using UniformVec3_sptr = std::shared_ptr<UniformVec3>;
using vecUniformVec3_sptr = std::vector<UniformVec3_sptr>;

class UniformVec3 : public Uniform<glm::vec3> {
public:
    UniformVec3(const glm::vec3 &value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() override;
};

#endif //JUMPERBALLAPPLICATION_UNIFORMVEC3_H
