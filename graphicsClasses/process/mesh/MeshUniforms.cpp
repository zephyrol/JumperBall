//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "MeshUniforms.h"

MeshUniforms::MeshUniforms(Uniforms uniforms) :
    _uniforms(std::move(uniforms)) {
}

MeshUniforms MeshUniforms::createInstance(
    const CstDisplayable_sptr &displayable,
    const CstShaderProgram_sptr &shaderProgram
) {

    auto uniformVec4s = createUniformsFromDynamics<UniformVec4>(
        displayable->getDynamicQuaternionValues(),
        displayable->getDynamicQuaternionNames(),
        shaderProgram
    );

    auto uniformVec3s = createUniformsFromDynamics<UniformVec3>(
        displayable->getDynamicVec3fValues(),
        displayable->getDynamicVec3fNames(),
        shaderProgram
    );

    auto uniformVec2s = createUniformsFromDynamics<UniformVec2>(
        displayable->getDynamicVec2fValues(),
        displayable->getDynamicVec2fNames(),
        shaderProgram
    );

    auto uniformFloats = createUniformsFromDynamics<UniformFloat>(
        displayable->getDynamicFloatValues(),
        displayable->getDynamicFloatNames(),
        shaderProgram
    );

    auto uniformInts = createUniformsFromDynamics<UniformInt>(
        displayable->getDynamicIntValues(),
        displayable->getDynamicIntNames(),
        shaderProgram
    );

    Uniforms uniforms(
        std::move(uniformVec4s),
        std::move(uniformVec3s),
        std::move(uniformVec2s),
        std::move(uniformFloats),
        std::move(uniformInts)
    );
}

