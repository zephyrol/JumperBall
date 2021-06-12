/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

RenderPass::RenderPass(const vecMesh_sptr& meshes):
    _meshes(meshes),
    _unitedMeshesGroup(std::make_shared <RenderGroup>(meshes, ObjectState::GlobalState::United)),
    _separateMeshGroups(createSeparateMeshGroups(meshes)),
    _renderGroupsUniforms{},
    _renderPassUniforms(),
    _renderPassUniformBlocks{} {
}

void RenderPass::update() {

    const auto updateRenderGroupUniforms =
        [this] (const std::shared_ptr <RenderGroup>& renderGroup) {
            const CstMesh_sptr headMesh = renderGroup->getHeadMesh();
            if (headMesh) {
                _renderGroupsUniforms[renderGroup] = headMesh->genUniformsValues();
            }
        };

    const vecMesh_sptr rejectedMeshes = _unitedMeshesGroup->update();
    updateRenderGroupUniforms(_unitedMeshesGroup);

    for (const auto& separateMeshGroup : _separateMeshGroups) {
        const Mesh_sptr& separateMesh = separateMeshGroup.first;
        const std::shared_ptr <RenderGroup>& separateRenderGroup = separateMeshGroup.second;

        bool foundRejectedMesh = false;
        for (const Mesh_sptr& rejectedMesh : rejectedMeshes) {
            if (rejectedMesh.get() == separateMesh.get()) {
                foundRejectedMesh = true;
            }
        }
        if (foundRejectedMesh) {
            separateRenderGroup->update({ separateMesh });
        } else {
            separateRenderGroup->update();
        }
        updateRenderGroupUniforms(separateRenderGroup);
    }
}

void RenderPass::cleanUniforms() {
    _renderGroupsUniforms = {};
    _renderPassUniformBlocks = {};
    _renderPassUniforms = {};
}

template<typename T> void RenderPass::upsertUniforms (const std::map <std::string, T>& uniformsData) {
    for (const auto& uniform : uniformsData) {
        upsertUniform(uniform.first, uniform.second);
    }
}

std::map <Mesh_sptr, std::shared_ptr <RenderGroup> > RenderPass::createSeparateMeshGroups (
    const vecMesh_sptr& meshes) const {
    std::map <Mesh_sptr, std::shared_ptr <RenderGroup> > separateMeshGroups;
    for (const Mesh_sptr& mesh : meshes) {
        separateMeshGroups[mesh] = std::make_shared <RenderGroup>(
            *_unitedMeshesGroup,
            ObjectState::GlobalState::Separate
            );
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

void RenderPass::upsertUniformTexture (GLuint shaderProgramID, const std::string& name, const GLuint& value) {
    _renderPassUniforms[shaderProgramID].uniformTextures[name] = value;
}

void RenderPass::upsertUniform (
    GLuint shaderProgramID,
    const std::string& name,
    const std::shared_ptr <const UniformBlock>& uniformBlock
    ) {
    _renderPassUniformBlocks[shaderProgramID][name] = uniformBlock;
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

    int textureNumber = 0;
    for (const auto& uniformTexture : uniforms.uniformTextures) {
        shaderProgram->bindUniformTexture(uniformTexture.first, textureNumber, uniformTexture.second);
        ++textureNumber;
    }
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
    if (_renderPassUniformBlocks.find(shaderProgramID) != _renderPassUniformBlocks.end()) {
        for (const auto& uniformBlock : _renderPassUniformBlocks.at(shaderProgramID)) {
            uniformBlock.second->bind(shaderProgram);
        }
    }

    if (_renderPassUniforms.find(shaderProgramID) != _renderPassUniforms.end()) {
        bindUniforms(_renderPassUniforms.at(shaderProgramID), shaderProgram);
    }

    for (const auto& renderGroupUniforms : _renderGroupsUniforms) {
        bindUniforms(renderGroupUniforms.second, shaderProgram);
        renderGroupUniforms.first->render();
    }
}
