//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "Uniforms.h"

Uniforms::Uniforms(
    vecUniformVec4_uptr uniformsVec4,
    vecUniformVec3_uptr uniformsVec3,
    vecUniformVec2_uptr uniformsVec2,
    vecUniformFloat_uptr uniformsFloat,
    vecUniformInt_uptr uniformsInt
): _uniformsVec4(std::move(uniformsVec4)),
   _uniformsVec3(std::move(uniformsVec3)),
   _uniformsVec2(std::move(uniformsVec2)),
   _uniformsFloat(std::move(uniformsFloat)),
   _uniformsInt(std::move(uniformsInt))
{
}
