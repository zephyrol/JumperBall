//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_MESHUNIFORMS_H
#define JUMPERBALLAPPLICATION_MESHUNIFORMS_H

#include "scene/Displayable.h"
#include "ShaderProgram.h"
#include "process/mesh/uniform/UniformVec4.h"
#include "process/mesh/uniform/UniformVec3.h"
#include "process/mesh/uniform/UniformVec2.h"
#include "process/mesh/uniform/UniformFloat.h"
#include "process/mesh/uniform/UniformInt.h"

class MeshUniforms;

using MeshUniforms_sptr = std::shared_ptr<MeshUniforms>;
using CstMeshUniforms_sptr = std::shared_ptr<const MeshUniforms>;
using vecCstMeshUniforms_sptr = std::vector<CstMeshUniforms_sptr>;
using vecMeshUniforms_sptr = std::vector<MeshUniforms_sptr>;

class MeshUniforms {
public:

    static MeshUniforms createInstance(
        const CstDisplayable_sptr &displayable,
        const CstShaderProgram_sptr &shaderProgram,
        short dynamicsId
    );

    explicit MeshUniforms(
        CstDisplayable_sptr displayable,
        vecUniformVec4_sptr uniformsVec4,
        vecUniformVec3_sptr uniformsVec3,
        vecUniformVec2_sptr uniformsVec2,
        vecUniformFloat_sptr uniformsFloat,
        vecUniformInt_sptr uniformsInt
    );

    void bind() const;

    void update();

private:
    const CstDisplayable_sptr _displayable;
    const vecUniformVec4_sptr _uniformsVec4;
    const vecUniformVec3_sptr _uniformsVec3;
    const vecUniformVec2_sptr _uniformsVec2;
    const vecUniformFloat_sptr _uniformsFloat;
    const vecUniformInt_sptr _uniformsInt;
    const vecUniformBase_sptr _uniformsToBind;

    template<typename UniformType, typename DynamicType>
    static std::vector<std::shared_ptr<UniformType>> createUniformsFromDynamics(
        std::vector<DynamicType> dynamics,
        const std::vector<std::string> &uniformNames,
        short dynamicsId,
        const CstShaderProgram_sptr &shaderProgram
    );

    template<typename T1, typename T2>
    void updateUniforms(
        const std::vector<T1> &newValues,
        T2& currentUniforms
    );
};


template<typename UniformType, typename DynamicType>
std::vector<std::shared_ptr<UniformType>> MeshUniforms::createUniformsFromDynamics(
    std::vector<DynamicType> dynamics,
    const std::vector<std::string> &uniformNames,
    short dynamicsId,
    const CstShaderProgram_sptr &shaderProgram
) {
    std::vector<std::shared_ptr<UniformType>> uniforms;

    for (size_t i = 0; i < dynamics.size(); ++i) {
        uniforms.emplace_back(std::make_shared<UniformType>(
            Utility::convertToOpenGLFormat(dynamics[i]),
            shaderProgram,
            uniformNames[i] + "[" + std::to_string(dynamicsId) + "]"
        ));
    }
    return uniforms;
}

template<typename T1, typename T2>
void MeshUniforms::updateUniforms(const std::vector<T1> &newValues, T2 &currentUniforms) {
    for (size_t i = 0; i < currentUniforms.size(); ++i) {
        auto &currentUniform = currentUniforms[i];
        const auto newValue = Utility::convertToOpenGLFormat(newValues[i]);
        currentUniform->update(newValue);
    }
}


#endif //JUMPERBALLAPPLICATION_MESHUNIFORMS_H
