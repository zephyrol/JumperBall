//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMINT_H
#define JUMPERBALLAPPLICATION_UNIFORMINT_H

#include "Uniform.h"

class UniformInt;

using UniformInt_uptr = std::unique_ptr<UniformInt>;
using vecUniformInt_uptr = std::vector<UniformInt_uptr>;

class UniformInt : public Uniform<GLint> {

public:
    UniformInt(GLint value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() override;
};


#endif //JUMPERBALLAPPLICATION_UNIFORMINT_H
