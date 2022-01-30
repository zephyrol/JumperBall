/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

RenderPass::RenderPass(const vecMesh_sptr& meshes):
    _meshes(meshes),
    _updatableMeshes(createUpdatableMeshes()),
    _meshStates(createMeshStates()),
    _unitedMeshesGroup(createUnitedMeshesGroup()),
    _separateMeshGroups(createSeparateMeshGroups()),
    _renderGroupsUniforms{},
    _renderPassUniforms()
{
}


void RenderPass::update() {

    const auto updateRenderGroupUniforms =
        [this] (const std::shared_ptr <RenderGroup>& renderGroup) {
            const CstMesh_sptr headMesh = renderGroup->getHeadMesh();
            _renderGroupsUniforms[renderGroup] = headMesh->genUniformsValues();
        };

    bool needsGroupsRedefinition = false;
    for(const auto& updatableMesh: _updatableMeshes) {
        if(updatableMesh->getGlobalState() != _meshStates.at(updatableMesh)) {
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

    for (const auto& separateMeshGroup : _separateMeshGroups) {
        const std::shared_ptr <RenderGroup>& separateRenderGroup = separateMeshGroup.second;
        updateRenderGroupUniforms(separateRenderGroup);
    }
}

template<typename T> void RenderPass::upsertUniforms (const std::unordered_map <std::string, T>& uniformsData) {
    for (const auto& uniform : uniformsData) {
        upsertUniform(uniform.first, uniform.second);
    }
}

std::unordered_map <Mesh_sptr, std::shared_ptr <RenderGroup> > RenderPass::createSeparateMeshGroups () const {
    std::unordered_map <Mesh_sptr, std::shared_ptr <RenderGroup> > separateMeshGroups;
    for (const Mesh_sptr& mesh : _meshes) {
        if(mesh->getGlobalState() == SceneElement::GlobalState::Separate) {
            separateMeshGroups[mesh] = std::make_shared <RenderGroup>(
                    std::initializer_list<Mesh_sptr>({mesh})
            );
        }
    }
    return separateMeshGroups;
}

void RenderPass::upsertUniform (GLuint shaderProgramID, const std::string& name, const glm::mat4& value) {
    _renderPassUniforms[shaderProgramID].uniformMat4s[name] = value;
}

void RenderPass::upsertUniform (GLuint shaderProgramID, const std::string& name, const glm::vec4& value) {
    _renderPassUniforms[shaderProgramID].uniformVec4s[name] = value;
}

void RenderPass::upsertUniform (GLuint shaderProgramID, const std::string& name, const glm::vec3& value) {
    _renderPassUniforms[shaderProgramID].uniformVec3s[name] = value;
}

void RenderPass::upsertUniform (GLuint shaderProgramID, const std::string& name, const glm::vec2& value) {
    _renderPassUniforms[shaderProgramID].uniformVec2s[name] = value;
}

void RenderPass::upsertUniform (GLuint shaderProgramID, const std::string& name, const GLfloat& value) {
    _renderPassUniforms[shaderProgramID].uniformFloats[name] = value;
}

void RenderPass::bindUniforms (
    const Mesh::Uniforms& uniforms,
    const CstShaderProgram_sptr& shaderProgram
) const {

    bindUniformVariables(uniforms.uniformFloats, shaderProgram);
    bindUniformVariables(uniforms.uniformVec2s, shaderProgram);
    bindUniformVariables(uniforms.uniformVec3s, shaderProgram);
    bindUniformVariables(uniforms.uniformVec4s, shaderProgram);
    bindUniformVariables(uniforms.uniformMat4s, shaderProgram);
}

template<typename T> void RenderPass::bindUniformVariables (
    Mesh::UniformVariables <T> uniforms,
    const CstShaderProgram_sptr& shaderProgram
    ) const {
    for (const auto& uniform : uniforms) {
        shaderProgram->bindUniform(uniform.first, uniform.second);
    }
}

void RenderPass::render (const CstShaderProgram_sptr& shaderProgram) const {
    const GLuint shaderProgramID = shaderProgram->getHandle();

    if (_renderPassUniforms.find(shaderProgramID) != _renderPassUniforms.end()) {
        bindUniforms(_renderPassUniforms.at(shaderProgramID), shaderProgram);
    }

    for (const auto& renderGroupUniforms : _renderGroupsUniforms) {
        bindUniforms(renderGroupUniforms.second, shaderProgram);
        renderGroupUniforms.first->render();
    }
}

vecMesh_sptr RenderPass::createUpdatableMeshes() const {
    vecMesh_sptr updatableMeshes {};
    for (const auto &mesh : _meshes) {
        if (!mesh->updatingIsUseless()) {
            updatableMeshes.push_back(mesh);
        }
    }
    return updatableMeshes;
}

std::map<Mesh_sptr, SceneElement::GlobalState> RenderPass::createMeshStates() const {
    std::map<Mesh_sptr, SceneElement::GlobalState> meshStates {};
    for(const auto& updatableMesh: _updatableMeshes) {
        meshStates[updatableMesh] = updatableMesh->getGlobalState();
    }
    return meshStates;
}

std::shared_ptr<RenderGroup> RenderPass::createUnitedMeshesGroup() const {
    vecMesh_sptr unitedMeshes {};
    for(const auto& mesh: _meshes) {
        if (mesh->getGlobalState() == SceneElement::GlobalState::United) {
            unitedMeshes.push_back(mesh);
        }
    }
    return !unitedMeshes.empty() ? std::make_shared<RenderGroup>(unitedMeshes) : nullptr;
}

void RenderPass::freeGPUMemory() {
    if(_unitedMeshesGroup) {
        _unitedMeshesGroup->freeGPUMemory();
    }
    for (const auto& separateMeshGroup: _separateMeshGroups){
        const auto& renderGroup = separateMeshGroup.second;
        renderGroup->freeGPUMemory();
    }
}


