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

    RenderPass(vecMesh_sptr meshes);

    RenderPass(const RenderPass &renderPass) = delete;

    RenderPass &operator=(const RenderPass &renderPass) = delete;

    RenderPass(RenderPass &&renderPass) = default;


    void render(const CstShaderProgram_sptr &shaderProgram) const;

    void update();

    void freeGPUMemory();

private:

    void bindUniforms(const Mesh::Uniforms &uniforms, const CstShaderProgram_sptr &shaderProgram) const;

    template<typename T>
    void bindUniformVariables(
        Mesh::UniformVariables<T> uniforms,
        const CstShaderProgram_sptr &shaderProgram
    ) const;

    template<typename T>
    void upsertUniforms(const std::unordered_map<std::string, T> &uniformsData);

    std::unordered_map<Mesh_sptr, std::shared_ptr<RenderGroup> > createSeparateMeshGroups() const;

    vecMesh_sptr createUpdatableMeshes() const;

    std::map<Mesh_sptr, Displayable::GlobalState> createMeshStates() const;

    std::shared_ptr<RenderGroup> createUnitedMeshesGroup() const;


    const vecMesh_sptr _meshes;
    const vecMesh_sptr _updatableMeshes;
    std::map<Mesh_sptr, Displayable::GlobalState> _meshStates;

    std::shared_ptr<RenderGroup> _unitedMeshesGroup;
    std::unordered_map<Mesh_sptr, std::shared_ptr<RenderGroup> > _separateMeshGroups;

    std::unordered_map<std::shared_ptr<RenderGroup>, Mesh::Uniforms> _renderGroupsUniforms;
};


#endif /* RENDER_PASS_H */
