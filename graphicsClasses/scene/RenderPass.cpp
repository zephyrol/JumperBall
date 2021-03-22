/*
 * File: RenderPass.cpp
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#include "RenderPass.h"

RenderPass::RenderPass(const ShaderProgram& shaderProgram, const vecMesh_sptr& meshes):
    _shaderProgram(shaderProgram),
    _meshes(meshes),
    _unitedMeshesGroup(std::make_shared <RenderGroup>(meshes, State::GlobalState::United)),
    _separateMeshGroups(createSeparateMeshGroups(meshes)),
    _renderGroupsUniforms{},
    _renderPassUniforms() {
}

void RenderPass::update() {

    const auto updateRenderGroupUniforms =
        [this] (const std::shared_ptr <RenderGroup>& renderGroup) {
            const CstMesh_sptr headMesh = renderGroup->getHeadMesh();
            if (headMesh) {
              _renderGroupsUniforms[renderGroup] = headMesh->genUniformsValues();
            }
        };

    vecMesh_sptr rejectedMeshes = _unitedMeshesGroup->update();
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
            State::GlobalState::Separate
            );
    }
    return separateMeshGroups;
}

void RenderPass::upsertUniform (const std::string& name, const glm::mat4& value) {
    _renderPassUniforms.uniformMat4s[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const glm::vec4& value) {
    _renderPassUniforms.uniformVec4s[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const glm::vec3& value) {
    _renderPassUniforms.uniformVec3s[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const glm::vec2& value) {
    _renderPassUniforms.uniformVec2s[name] = value;
}

void RenderPass::upsertUniform (const std::string& name, const GLfloat& value) {
    _renderPassUniforms.uniformFloats[name] = value;
}

void RenderPass::upsertUniformTexture (const std::string& name, const GLuint value) {
    _renderPassUniforms.uniformTextures[name] = value;
}

void RenderPass::bindUniforms (const Mesh::Uniforms& uniforms) const {

    bindUniforms(uniforms.uniformFloats);
    bindUniforms(uniforms.uniformVec2s);
    bindUniforms(uniforms.uniformVec3s);
    bindUniforms(uniforms.uniformVec4s);
    bindUniforms(uniforms.uniformMat4s);

    int textureNumber = 0;
    for (const auto& uniformTexture : uniforms.uniformTextures) {
        _shaderProgram.bindUniformTexture(uniformTexture.first, textureNumber, uniformTexture.second);
        ++textureNumber;
    }
}

void RenderPass::render() const {
    _shaderProgram.use();
    bindUniforms(_renderPassUniforms);
    int i = 0;
    for (const auto& renderGroupUniforms : _renderGroupsUniforms) {
        bindUniforms(renderGroupUniforms.second);
        renderGroupUniforms.first->render();
    }
}

template<typename T> void RenderPass::bindUniforms (Mesh::UniformVariable <T> uniforms) const {
    for (const auto& uniform : uniforms) {
        _shaderProgram.bindUniform(uniform.first, uniform.second);
    }
}
