//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMFLOAT_H
#define JUMPERBALLAPPLICATION_UNIFORMFLOAT_H

#include "Uniform.h"

class UniformFloat;

using UniformFloat_sptr = std::shared_ptr<UniformFloat>;
using vecUniformFloat_sptr = std::vector<UniformFloat_sptr>;

class UniformFloat : public Uniform<GLfloat> {
   public:
    UniformFloat(GLfloat value, const ShaderProgram_uptr& shaderProgram, const std::string& name);

    void bind() const override;
};

#endif  // JUMPERBALLAPPLICATION_UNIFORMFLOAT_H
