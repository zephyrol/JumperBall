//
// Created by S.Morgenthaler on 22/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_MESHUNIFORMS_H
#define JUMPERBALLAPPLICATION_MESHUNIFORMS_H

#include "process/mesh/uniform/Uniforms.h"
#include "scene/Displayable.h"

class MeshUniforms {
public:
    static MeshUniforms createInstance(
        const CstDisplayable_sptr &displayable,
        const CstShaderProgram_sptr &shaderProgram
    );

    explicit MeshUniforms(Uniforms uniforms);
    void bind();

private:
    Uniforms _uniforms;

    template<typename UniformType, typename DynamicType>
    static std::vector<std::unique_ptr<UniformType>> createUniformsFromDynamics(
        std::vector<DynamicType> dynamics,
        const std::vector<std::string> &uniformNames,
        const CstShaderProgram_sptr &shaderProgram
    );
};

template<typename UniformType, typename DynamicType>
std::vector<std::unique_ptr<UniformType>> MeshUniforms::createUniformsFromDynamics(
    std::vector<DynamicType> dynamics,
    const std::vector<std::string> &uniformNames,
    const CstShaderProgram_sptr &shaderProgram
) {
    std::vector<std::unique_ptr<UniformType>> uniforms;

    for(size_t i = 0; i < dynamics.size(); ++i) {
        uniforms.emplace_back(std::unique_ptr<UniformType>(new UniformType(
            Utility::convertToOpenGLFormat(dynamics[i]),
            shaderProgram,
            uniformNames[i]
        )));
    }
    return uniforms;
}


#endif //JUMPERBALLAPPLICATION_MESHUNIFORMS_H
