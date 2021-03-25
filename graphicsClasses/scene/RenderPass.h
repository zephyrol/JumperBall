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
#include "FrameBuffer.h"
#include "RenderGroup.h"
#include "uniformBlocks/uniformBlock.h"

class RenderPass {

public:

RenderPass(const ShaderProgram& shaderProgram, const vecMesh_sptr& meshes);
RenderPass(const RenderPass& renderPass) = delete;
RenderPass& operator= (const RenderPass& renderPass) = delete;
RenderPass(RenderPass&& renderPass) = default;
RenderPass& operator= (RenderPass&& renderPass) = default;


void render() const;
void update();
void cleanUniforms();
void upsertUniform(const std::string& name, const glm::mat4& value);
void upsertUniform(const std::string& name, const glm::vec4& value);
void upsertUniform(const std::string& name, const glm::vec3& value);
void upsertUniform(const std::string& name, const glm::vec2& value);
void upsertUniform(const std::string& name, const GLfloat& value);
void upsertUniformTexture(const std::string& name, const GLuint value);
void upsertUniform(const std::string& name, const std::shared_ptr <const UniformBlock>& uniformBlock);

private:

void bindUniforms(const Mesh::Uniforms& uniforms) const;
template<typename T> void bindUniforms(Mesh::UniformVariable <T> uniforms) const;
template<typename T> void upsertUniforms(const std::map <std::string, T>& uniformsData);
std::map <Mesh_sptr, std::shared_ptr <RenderGroup> > createSeparateMeshGroups(
    const vecMesh_sptr& meshes
    ) const;

const ShaderProgram& _shaderProgram;
const vecMesh_sptr _meshes;
const std::shared_ptr <RenderGroup> _unitedMeshesGroup;
std::map <Mesh_sptr, std::shared_ptr <RenderGroup> > _separateMeshGroups;
std::map <std::shared_ptr <RenderGroup>, Mesh::Uniforms> _renderGroupsUniforms;
Mesh::Uniforms _renderPassUniforms;
std::map <std::string, std::shared_ptr <const UniformBlock> > _renderPassUniformBlocks;
};


#endif /* RENDER_PASS_H */
