//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMS_H
#define JUMPERBALLAPPLICATION_UNIFORMS_H


#include "UniformVec4.h"
#include "UniformVec3.h"
#include "UniformVec2.h"
#include "UniformFloat.h"
#include "UniformInt.h"

class Uniforms {

public:

    explicit Uniforms(
        vecUniformVec4_uptr uniformsVec4,
        vecUniformVec3_uptr uniformsVec3,
        vecUniformVec2_uptr uniformsVec2,
        vecUniformFloat_uptr uniformsFloat,
        vecUniformInt_uptr uniformsInt
    );

private:

    const vecUniformVec4_uptr _uniformsVec4;
    const vecUniformVec3_uptr _uniformsVec3;
    const vecUniformVec2_uptr _uniformsVec2;
    const vecUniformFloat_uptr _uniformsFloat;
    const vecUniformInt_uptr _uniformsInt;
};


#endif //JUMPERBALLAPPLICATION_UNIFORMS_H
