//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMFLOAT_H
#define JUMPERBALLAPPLICATION_UNIFORMFLOAT_H


#include "Uniform.h"

class UniformFloat;

using UniformFloat_uptr = std::unique_ptr<UniformFloat>;
using vecUniformFloat_uptr = std::vector<UniformFloat_uptr>;

class UniformFloat : public Uniform<GLfloat> {
public:

    UniformFloat(GLfloat value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() override;
};


#endif //JUMPERBALLAPPLICATION_UNIFORMFLOAT_H
