//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMINT_H
#define JUMPERBALLAPPLICATION_UNIFORMINT_H

#include "Uniform.h"

class UniformInt;

using UniformInt_sptr = std::shared_ptr<UniformInt>;
using vecUniformInt_sptr = std::vector<UniformInt_sptr>;

class UniformInt : public Uniform<GLint> {

public:
    UniformInt(GLint value, const CstShaderProgram_sptr &shaderProgram, const std::string &name);

    void bind() const override;
};


#endif //JUMPERBALLAPPLICATION_UNIFORMINT_H
