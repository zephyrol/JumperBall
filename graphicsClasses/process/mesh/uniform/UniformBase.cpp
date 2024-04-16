//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "UniformBase.h"

UniformBase::UniformBase(const ShaderProgram_uptr& shaderProgram, const std::string& name)
    : _location(glGetUniformLocation(shaderProgram->getHandle(), name.c_str())) {}
