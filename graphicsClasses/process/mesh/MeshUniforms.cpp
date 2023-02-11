//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "MeshUniforms.h"

#include <utility>

MeshUniforms::MeshUniforms(
    CstDisplayable_sptr displayable,
    vecUniformVec4_sptr uniformsVec4,
    vecUniformVec3_sptr uniformsVec3,
    vecUniformVec2_sptr uniformsVec2,
    vecUniformFloat_sptr uniformsFloat,
    vecUniformInt_sptr uniformsInt
) : _displayable(std::move(displayable)),
    _uniformsVec4(std::move(uniformsVec4)),
    _uniformsVec3(std::move(uniformsVec3)),
    _uniformsVec2(std::move(uniformsVec2)),
    _uniformsFloat(std::move(uniformsFloat)),
    _uniformsInt(std::move(uniformsInt)),
    _uniformsToBind([this]() {
        vecUniformBase_sptr uniforms{};
        for (const auto &uniform: _uniformsVec4) {
            uniforms.emplace_back(uniform);
        }
        for (const auto &uniform: _uniformsVec3) {
            uniforms.emplace_back(uniform);
        }
        for (const auto &uniform: _uniformsVec2) {
            uniforms.emplace_back(uniform);
        }
        for (const auto &uniform: _uniformsFloat) {
            uniforms.emplace_back(uniform);
        }
        for (const auto &uniform: _uniformsInt) {
            uniforms.emplace_back(uniform);
        }
        return uniforms;
    }()) {
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

    return MeshUniforms(
        displayable,
        std::move(uniformVec4s),
        std::move(uniformVec3s),
        std::move(uniformVec2s),
        std::move(uniformFloats),
        std::move(uniformInts)
    );
}

void MeshUniforms::update() {
    updateUniforms(_displayable->getDynamicQuaternionValues(), _uniformsVec4);
    updateUniforms(_displayable->getDynamicVec3fValues(), _uniformsVec3);
    updateUniforms(_displayable->getDynamicVec2fValues(), _uniformsVec2);
    updateUniforms(_displayable->getDynamicFloatValues(), _uniformsFloat);
    updateUniforms(_displayable->getDynamicIntValues(), _uniformsInt);
}

void MeshUniforms::bind() const {
    for (const auto &uniform: _uniformsToBind) {
        uniform->bind();
    }
}
