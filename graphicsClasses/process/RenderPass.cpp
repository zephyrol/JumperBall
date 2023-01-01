/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

#include <utility>

RenderPass::RenderPass(vecMesh_sptr meshes) :
    _meshes(std::move(meshes)),
    _updatableMeshes(createUpdatableMeshes()),
    _meshStates(createMeshStates()),
    _unitedMeshesGroup(createUnitedMeshesGroup()),
    _separateMeshGroups(createSeparateMeshGroups()),
    _renderGroupsUniforms{} {
}

void RenderPass::update() {

    const auto updateRenderGroupUniforms =
        [this](const std::shared_ptr<RenderGroup> &renderGroup) {
            const CstMesh_sptr headMesh = renderGroup->getHeadMesh();
            _renderGroupsUniforms[renderGroup] = headMesh->genUniformsValues();
        };

    bool needsGroupsRedefinition = false;
    for (const auto &updatableMesh: _updatableMeshes) {
        if (updatableMesh->getGlobalState() != _meshStates.at(updatableMesh)) {
            needsGroupsRedefinition = true;
        }
    }
    if (needsGroupsRedefinition) {
        freeGPUMemory();
        _meshStates = createMeshStates();
        _unitedMeshesGroup = createUnitedMeshesGroup();
        _separateMeshGroups = createSeparateMeshGroups();
        _renderGroupsUniforms.clear();
    }

    if (_unitedMeshesGroup) {
        updateRenderGroupUniforms(_unitedMeshesGroup);
    }

    for (const auto &separateMeshGroup: _separateMeshGroups) {
        const std::shared_ptr<RenderGroup> &separateRenderGroup = separateMeshGroup.second;
        updateRenderGroupUniforms(separateRenderGroup);
    }
}

template<typename T>
void RenderPass::upsertUniforms(const std::unordered_map<std::string, T> &uniformsData) {
    for (const auto &uniform: uniformsData) {
        upsertUniform(uniform.first, uniform.second);
    }
}

std::unordered_map<Mesh_sptr, std::shared_ptr<RenderGroup> > RenderPass::createSeparateMeshGroups() const {
    std::unordered_map<Mesh_sptr, std::shared_ptr<RenderGroup> > separateMeshGroups;
    for (const Mesh_sptr &mesh: _meshes) {
        if (mesh->getGlobalState() == Displayable::GlobalState::Separate) {
            separateMeshGroups[mesh] = std::make_shared<RenderGroup>(
                std::initializer_list<Mesh_sptr>({mesh})
            );
        }
    }
    return separateMeshGroups;
}

void RenderPass::bindUniforms(
    const Mesh::Uniforms &uniforms,
    const CstShaderProgram_sptr &shaderProgram
) const {

    bindUniformVariables(uniforms.uniformInts, shaderProgram);
    bindUniformVariables(uniforms.uniformFloats, shaderProgram);
    bindUniformVariables(uniforms.uniformVec2s, shaderProgram);
    bindUniformVariables(uniforms.uniformVec3s, shaderProgram);
    bindUniformVariables(uniforms.uniformVec4s, shaderProgram);
    bindUniformVariables(uniforms.uniformMat4s, shaderProgram);
}

template<typename T>
void RenderPass::bindUniformVariables(
    Mesh::UniformVariables<T> uniforms,
    const CstShaderProgram_sptr &shaderProgram
) const {
    for (const auto &uniform: uniforms) {
        shaderProgram->bindUniform(uniform.first, uniform.second);
    }
}

void RenderPass::render(const CstShaderProgram_sptr &shaderProgram) const {

    if (_unitedMeshesGroup) {
        bindUniforms(_renderGroupsUniforms.at(_unitedMeshesGroup), shaderProgram);
        _unitedMeshesGroup->render();
    }
    for (const auto &renderGroup: _separateMeshGroups) {
        bindUniforms(_renderGroupsUniforms.at(renderGroup.second), shaderProgram);
        renderGroup.second->render();
    }
}

vecMesh_sptr RenderPass::createUpdatableMeshes() const {
    vecMesh_sptr updatableMeshes{};
    for (const auto &mesh: _meshes) {
        updatableMeshes.push_back(mesh);
        if (!mesh->updatingIsUseless()) {
            updatableMeshes.push_back(mesh);
        }
    }
    return updatableMeshes;
}

std::map<Mesh_sptr, Displayable::GlobalState> RenderPass::createMeshStates() const {
    std::map<Mesh_sptr, Displayable::GlobalState> meshStates{};
    for (const auto &updatableMesh: _updatableMeshes) {
        meshStates[updatableMesh] = updatableMesh->getGlobalState();
    }
    return meshStates;
}

std::shared_ptr<RenderGroup> RenderPass::createUnitedMeshesGroup() const {
    vecMesh_sptr unitedMeshes{};
    for (const auto &mesh: _meshes) {
        if (mesh->getGlobalState() == Displayable::GlobalState::United) {
            unitedMeshes.push_back(mesh);
        }
    }
    return !unitedMeshes.empty() ? std::make_shared<RenderGroup>(unitedMeshes) : nullptr;
}

void RenderPass::freeGPUMemory() {
    if (_unitedMeshesGroup) {
        _unitedMeshesGroup->freeGPUMemory();
    }
    for (const auto &separateMeshGroup: _separateMeshGroups) {
        const auto &renderGroup = separateMeshGroup.second;
        renderGroup->freeGPUMemory();
    }
}


