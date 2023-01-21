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
    _uniformsNames(_meshes.empty() ? Mesh::UniformsNames() : _meshes.front()->genUniformsNames()),
    _updatableMeshesStates(std::accumulate(
        _meshes.begin(),
        _meshes.end(),
        std::vector<RenderPass::UpdatableMeshState>(),
        [](std::vector<RenderPass::UpdatableMeshState> &updatableMeshes, const Mesh_sptr &mesh) {
            if (!mesh->updatingIsUseless()) {
                updatableMeshes.push_back({mesh, mesh->getGlobalState()});
            }
            return updatableMeshes;
        })),
    _drawCallDefinitions(createDrawCallDefinitions())
{
}

void RenderPass::update() {

    bool needsGroupsRedefinition = false;
    for (const auto &updatableMeshState: _updatableMeshesStates) {
        if (updatableMeshState.mesh->getGlobalState() != updatableMeshState.currentState) {
            needsGroupsRedefinition = true;
        }
    }
    if (needsGroupsRedefinition) {
        freeGPUMemory();
        _drawCallDefinitions = createDrawCallDefinitions();
    }

    for(auto& drawCallDefinition: _drawCallDefinitions) {
        drawCallDefinition.uniformsValues = drawCallDefinition.renderGroup->genUniformValues();
    }

}

void RenderPass::bindUniforms(
    const Mesh::UniformsValues &uniforms,
    const CstShaderProgram_sptr &shaderProgram
) const {
    bindUniformVariables(shaderProgram, _uniformsNames.uniformsInts, uniforms.uniformInts);
    bindUniformVariables(shaderProgram, _uniformsNames.uniformFloats, uniforms.uniformFloats);
    bindUniformVariables(shaderProgram, _uniformsNames.uniformVec2s, uniforms.uniformVec2s);
    bindUniformVariables(shaderProgram, _uniformsNames.uniformVec3s, uniforms.uniformVec3s);
    bindUniformVariables(shaderProgram, _uniformsNames.uniformVec4s, uniforms.uniformVec4s);
}

void RenderPass::render(const CstShaderProgram_sptr &shaderProgram) const {
    for(const auto& drawCallDefinition: _drawCallDefinitions) {
        bindUniforms(drawCallDefinition.uniformsValues, shaderProgram);
        drawCallDefinition.renderGroup->render();
    }
}

void RenderPass::freeGPUMemory() {
    for(const auto& drawCallDefinition: _drawCallDefinitions) {
        drawCallDefinition.renderGroup->freeGPUMemory();
    }
}

std::vector<std::string> RenderPass::genUniformNames() const {
    if (_meshes.empty()) {
        return {};
    }
    // Every mesh defines the same uniforms.
    return _meshes.front()->genGatheredUniformsNames();
}

std::vector<RenderPass::DrawCallDefinition> RenderPass::createDrawCallDefinitions() const {
    std::vector<RenderPass::DrawCallDefinition> drawCallDefinitions;

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
    if(!unitedMeshes.empty()) {
        const CstMesh_sptr headMesh = unitedMeshes.front();
        drawCallDefinitions.push_back({
            std::make_shared<RenderGroup>(RenderGroup::createInstance(std::move(unitedMeshes))),
            headMesh->genUniformsValues()
        });
    }

    // 2. Create separated mesh groups
    for(const auto& separatedMesh: separatedMeshes) {
        drawCallDefinitions.push_back({
            std::make_shared<RenderGroup>(
                RenderGroup::createInstance(std::initializer_list<Mesh_sptr>({separatedMesh}))
            ),
            separatedMesh->genUniformsValues()
        });
    }

    return drawCallDefinitions;
}

