//
// Created by S.Morgenthaler on 02/07/2023.
//

#include "RenderGroupUniforms.h"

RenderGroupUniforms::RenderGroupUniforms(vecMeshUniforms_sptr&& dynamicUniforms)
    : _dynamicUniforms(std::move(dynamicUniforms)) {}

RenderGroupUniforms RenderGroupUniforms::createInstance(const MeshDynamicGroup& meshDynamicGroup,
                                                        const ShaderProgram_uptr& shaderProgram) {
    vecMeshUniforms_sptr dynamicUniforms;
    for (const auto& meshList : meshDynamicGroup.dynamicMeshes()) {
        // We only use the front because all those meshes shared the same dynamic(uniform) hash
        const auto meshUniforms =
            std::make_shared<MeshUniforms>(meshList.front()->genMeshUniforms(shaderProgram));
        dynamicUniforms.emplace_back(meshUniforms);
    }

    return {std::move(dynamicUniforms)};
}

void RenderGroupUniforms::bind() const {
    for (const auto& meshUniforms : _dynamicUniforms) {
        meshUniforms->bind();
    }
}

void RenderGroupUniforms::update() {
    for (const auto& meshUniforms : _dynamicUniforms) {
        meshUniforms->update();
    }
}
