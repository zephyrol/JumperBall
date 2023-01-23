//
// Created by S.Morgenthaler on 22/01/2023.
//

#include "Uniforms.h"

Uniforms::Uniforms(
    vecUniformVec4_sptr uniformsVec4,
    vecUniformVec3_sptr uniformsVec3,
    vecUniformVec2_sptr uniformsVec2,
    vecUniformFloat_sptr uniformsFloat,
    vecUniformInt_sptr uniformsInt
): _uniformsVec4(std::move(uniformsVec4)),
   _uniformsVec3(std::move(uniformsVec3)),
   _uniformsVec2(std::move(uniformsVec2)),
   _uniformsFloat(std::move(uniformsFloat)),
   _uniformsInt(std::move(uniformsInt)),
   _uniformsToBind([this](){
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
   }())
{
}

void Uniforms::bind() const {
    for(const auto& uniform: _uniformsToBind) {
        uniform->bind();
    }
}

void Uniforms::update(
    const std::vector<glm::vec4> &vec4s,
    const std::vector<glm::vec3> &vec3s,
    const std::vector<glm::vec2> &vec2s,
    const std::vector<GLfloat> &floats,
    const std::vector<GLint> &ints
) {
    _uniformsToBind.clear();
    updateUniforms(vec4s, _uniformsVec4);
}

