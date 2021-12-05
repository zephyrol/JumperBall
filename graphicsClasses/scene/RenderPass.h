/*
 * File: RenderPass.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#ifndef RENDER_PASS_H
#define RENDER_PASS_H
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include "FrameBuffer.h"
#include "RenderGroup.h"
#include "uniformBlocks/UniformBlock.h"

class RenderPass;
using RenderPass_sptr = std::shared_ptr <RenderPass>;
using CstRenderPass_sptr = std::shared_ptr <const RenderPass>;
using vecCstRenderPass_sptr = std::vector <CstRenderPass_sptr>;
using vecRenderPass_sptr = std::vector <RenderPass_sptr>;

class RenderPass {

public:

RenderPass(const vecMesh_sptr& meshes);
RenderPass(const RenderPass& renderPass) = delete;
RenderPass& operator= (const RenderPass& renderPass) = delete;
RenderPass(RenderPass&& renderPass) = default;
RenderPass& operator= (RenderPass&& renderPass) = default;


void render(const CstShaderProgram_sptr& shaderProgram) const;
void update();
void freeGPUMemory();

void cleanUniforms();
void upsertUniform(GLuint shaderProgramID, const std::string& name, const glm::mat4& value);
void upsertUniform(GLuint shaderProgramID, const std::string& name, const glm::vec4& value);
void upsertUniform(GLuint shaderProgramID, const std::string& name, const glm::vec3& value);
void upsertUniform(GLuint shaderProgramID, const std::string& name, const glm::vec2& value);
void upsertUniform(GLuint shaderProgramID, const std::string& name, const GLfloat& value);

void upsertUniform(GLuint shaderProgramID, const std::string& name, const GLuint& value) = delete;
void upsertUniformTexture(GLuint shaderProgramID, const std::string& name, const GLuint& value);

void upsertUniform(
    GLuint shaderProgramID,
    const std::string& name,
    const std::shared_ptr <const UniformBlock>& uniformBlock
    );

using UniformCstBlockVariables = std::unordered_map <std::string, CstUniformBlock_sptr>;

using UniformBlockVariables = std::unordered_map <std::string, UniformBlock_sptr>;
using UniformBlockVariables_uptr = std::unique_ptr <UniformBlockVariables>;

private:

void bindUniforms(const Mesh::Uniforms& uniforms, const CstShaderProgram_sptr& shaderProgram) const;

template<typename T> void bindUniformVariables(
    Mesh::UniformVariables <T> uniforms,
    const CstShaderProgram_sptr& shaderProgram
    ) const;
template<typename T> void upsertUniforms(const std::unordered_map <std::string, T>& uniformsData);

std::unordered_map <Mesh_sptr, std::shared_ptr <RenderGroup> > createSeparateMeshGroups(
    const vecMesh_sptr& meshes
    ) const;

const vecMesh_sptr _meshes;
const std::shared_ptr <RenderGroup> _unitedMeshesGroup;
std::unordered_map <Mesh_sptr, std::shared_ptr <RenderGroup> > _separateMeshGroups;
std::unordered_map <std::shared_ptr <RenderGroup>, Mesh::Uniforms> _renderGroupsUniforms;
std::unordered_map <GLuint, Mesh::Uniforms> _renderPassUniforms;
std::unordered_map <GLuint, UniformCstBlockVariables> _renderPassUniformBlocks;
};


#endif /* RENDER_PASS_H */
