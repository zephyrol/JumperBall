//
// Created by Sebastien Morgenthaler on 29/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_RENDERGROUPSMANAGER_H
#define JUMPERBALLAPPLICATION_RENDERGROUPSMANAGER_H


#include "process/mesh/Mesh.h"
#include "RenderGroup.h"

class RenderGroupsManager;
using RenderGroupsManager_sptr = std::shared_ptr<RenderGroupsManager>;
using CstRenderGroupsManager_sptr = std::shared_ptr<const RenderGroupsManager>;
using vecRenderGroupsManager_sptr = std::vector<RenderGroupsManager_sptr>;

/**
 * Render groups manager is used to merge and split render groups.
 * This is usable and shareable between some render passes. (and so with different shaders).
 * It doesn't own any shader programs. But it can generate uniforms from one.
 */
class RenderGroupsManager {

public:

    explicit RenderGroupsManager(vecMesh_sptr meshes);

    RenderGroupsManager(const RenderGroupsManager &renderPass) = delete;

    RenderGroupsManager &operator=(const RenderGroupsManager &renderPass) = delete;

    RenderGroupsManager(RenderGroupsManager &&renderPass) = default;

    /**
     * Update the render groups
     */
    void update();

    void freeGPUMemory();

    void render() const;

    std::shared_ptr<const std::vector<RenderGroup>> getRenderGroups() const;

    /**
     * Create uniforms adapted and corresponding to a shader program.
     * The shader program is not passed to the constructor of this class because a render group
     * could be shared by several shaders.
     */
    std::vector<MeshUniforms> genUniforms(const CstShaderProgram_sptr& shaderProgram) const;

private:

    const vecMesh_sptr _meshes;

    struct UpdatableMeshState {
        Mesh_sptr mesh;
        Displayable::GlobalState currentState;
    };
    std::vector<RenderGroupsManager::UpdatableMeshState> _updatableMeshesStates;

    std::shared_ptr<std::vector<RenderGroup>> _renderGroups;

    std::shared_ptr<std::vector<RenderGroup>> generateRenderGroups() const;
};


#endif //JUMPERBALLAPPLICATION_RENDERGROUPSMANAGER_H
