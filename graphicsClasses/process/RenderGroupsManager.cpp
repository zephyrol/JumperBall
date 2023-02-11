//
// Created by Sebastien Morgenthaler on 29/01/2023.
//

#include "RenderGroupsManager.h"

RenderGroupsManager::RenderGroupsManager(vecMesh_sptr meshes) :
    _meshes(std::move(meshes)),
    _updatableMeshesStates(std::accumulate(
        _meshes.begin(),
        _meshes.end(),
        std::vector<RenderGroupsManager::UpdatableMeshState>(),
        [](std::vector<RenderGroupsManager::UpdatableMeshState> &updatableMeshes, const Mesh_sptr &mesh) {
            if (!mesh->updatingIsUseless()) {
                updatableMeshes.push_back({mesh, mesh->getGlobalState()});
            }
            return updatableMeshes;
        })),
    _renderGroups(generateRenderGroups()) {
}

void RenderGroupsManager::update() {
    bool needsGroupsRedefinition = false;

    for (auto &updatableMeshState: _updatableMeshesStates) {
        const auto newMeshGlobalState = updatableMeshState.mesh->getGlobalState();
        if (newMeshGlobalState != updatableMeshState.currentState) {
            needsGroupsRedefinition = true;
            updatableMeshState.currentState = newMeshGlobalState;
        }
    }

    if (needsGroupsRedefinition) {
        freeGPUMemory();
        _renderGroups = generateRenderGroups();
    }
}

void RenderGroupsManager::freeGPUMemory() {
    for (auto &renderGroup: *_renderGroups) {
        renderGroup.freeGPUMemory();
    }
}

std::shared_ptr<std::vector<RenderGroup>> RenderGroupsManager::generateRenderGroups() const {
    std::vector<RenderGroup> renderGroups;

    vecMesh_sptr unitedMeshes{};
    vecMesh_sptr separatedMeshes{};
    for (const auto &mesh: _meshes) {
        if (mesh->getGlobalState() == Displayable::GlobalState::United) {
            unitedMeshes.push_back(mesh);
        } else if (mesh->getGlobalState() == Displayable::GlobalState::Separate) {
            separatedMeshes.push_back(mesh);
        }
    }

    // 1. Create united meshes group
    if (!unitedMeshes.empty()) {
        renderGroups.emplace_back(RenderGroup::createInstance(std::move(unitedMeshes)));
    }

    // 2. Create separated mesh groups
    for (const auto &separatedMesh: separatedMeshes) {
        renderGroups.emplace_back(
            RenderGroup::createInstance(std::initializer_list<Mesh_sptr>({separatedMesh}))
        );
    }

    return std::make_shared<std::vector<RenderGroup>>(std::move(renderGroups));
}

std::shared_ptr<const std::vector<RenderGroup>> RenderGroupsManager::getRenderGroups() const {
    return _renderGroups;
}

std::vector<MeshUniforms> RenderGroupsManager::genUniforms(const CstShaderProgram_sptr &shaderProgram) const {
    decltype(genUniforms(shaderProgram)) meshUniforms;
    for (const auto &renderGroup: *_renderGroups) {
        meshUniforms.emplace_back(renderGroup.genUniforms(shaderProgram));
    }
    return meshUniforms;
}
