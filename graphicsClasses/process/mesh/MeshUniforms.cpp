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

    const auto uniformNames = displayable->getDynamicNames();
    size_t uniformCount = 0;

    auto uniformVec4s = createUniformsFromDynamics<UniformVec4>(
        displayable->getDynamicQuaternionValues(),
        uniformNames,
        uniformCount,
        shaderProgram
    );

    auto uniformVec3s = createUniformsFromDynamics<UniformVec3>(
        displayable->getDynamicVec3fValues(),
        uniformNames,
        uniformCount,
        shaderProgram
    );

    auto uniformVec2s = createUniformsFromDynamics<UniformVec2>(
        displayable->getDynamicVec2fValues(),
        uniformNames,
        uniformCount,
        shaderProgram
    );

    auto uniformFloats = createUniformsFromDynamics<UniformFloat>(
        displayable->getDynamicFloatValues(),
        uniformNames,
        uniformCount,
        shaderProgram
    );

    auto uniformInts = createUniformsFromDynamics<UniformInt>(
        displayable->getDynamicIntValues(),
        uniformNames,
        uniformCount,
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

