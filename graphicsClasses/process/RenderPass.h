/*
 * File: RenderPass.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "FrameBuffer.h"
#include "RenderGroup.h"

class RenderPass;

using RenderPass_sptr = std::shared_ptr<RenderPass>;
using CstRenderPass_sptr = std::shared_ptr<const RenderPass>;
using vecCstRenderPass_sptr = std::vector<CstRenderPass_sptr>;
using vecRenderPass_sptr = std::vector<RenderPass_sptr>;

/**
 * Render passes is used to render meshes. They are usable and shareable between some render processes.
 * (and so with different shaders). It doesn't contain any shader programs but we need to specify it
 * during the rendering.
 */
class RenderPass {

public:

    explicit RenderPass(vecMesh_sptr meshes);

    RenderPass(const RenderPass &renderPass) = delete;

    RenderPass &operator=(const RenderPass &renderPass) = delete;

    RenderPass(RenderPass &&renderPass) = default;

    void render(const CstShaderProgram_sptr &shaderProgram) const;

    void update();

    void freeGPUMemory();

private:

    void bindUniforms(const Mesh::UniformsValues &uniforms, const CstShaderProgram_sptr &shaderProgram) const;

    template<typename T>
    void bindUniformVariables(
        const CstShaderProgram_sptr &shaderProgram,
        const Mesh::GpuUniformNames &uniformsNames,
        const Mesh::GpuUniformValues<T> &uniformsValues
    ) const;

    std::unordered_map<Mesh_sptr, std::shared_ptr<RenderGroup> > createSeparateMeshGroups() const;

    vecMesh_sptr createUpdatableMeshes() const;

    std::map<Mesh_sptr, Displayable::GlobalState> createMeshStates() const;

    std::shared_ptr<RenderGroup> createUnitedMeshesGroup() const;


    const vecMesh_sptr _meshes;
    const Mesh::UniformsNames _uniformsNames;
    const vecMesh_sptr _updatableMeshes;
    std::map<Mesh_sptr, Displayable::GlobalState> _meshStates;

    std::shared_ptr<RenderGroup> _unitedMeshesGroup;
    std::unordered_map<Mesh_sptr, std::shared_ptr<RenderGroup> > _separateMeshGroups;

    std::unordered_map<std::shared_ptr<RenderGroup>, Mesh::UniformsValues> _renderGroupsUniforms;
};

template<typename T>
void RenderPass::bindUniformVariables(
    const CstShaderProgram_sptr &shaderProgram,
    const Mesh::GpuUniformNames &uniformsNames,
    const Mesh::GpuUniformValues<T> &uniformsValues
) const {
    for (size_t i = 0; i < uniformsNames.size(); ++i) {
        shaderProgram->bindUniform(uniformsNames[i], uniformsValues[i]);
    }
}

#endif /* RENDER_PASS_H */
