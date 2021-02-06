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
#include "Camera.h"
#include "uniformBlocks/uniformLight.h"
#include "animations/BallAnimation.h"
#include "Mesh.h"

class RenderPass {

public:

RenderPass(const ShaderProgram& shaderProgram, const vecCstMesh_sptr& meshes);
RenderPass(const RenderPass& renderPass) = delete;
RenderPass& operator= (const RenderPass& renderPass) = delete;
RenderPass(RenderPass&& renderPass) = default;
RenderPass& operator= (RenderPass&& renderPass) = default;

template<typename T> using uniformVariable = std::map <std::string, T>;

template<typename T> void upsertVertexAttribute(size_t number, const std::vector <T>& data);

void render() const;

enum class StaticAttributeType { Positions, Normals, Colors, UvCoords, Indices };

void update();

void upsertUniform(const std::string& name, const glm::mat4& value);
void upsertUniform(const std::string& name, const glm::vec4& value);
void upsertUniform(const std::string& name, const glm::vec3& value);
void upsertUniform(const std::string& name, const glm::vec2& value);
void upsertUniform(const std::string& name, const GLfloat& value);
void upsertUniformTexture(const std::string& name, const GLuint value);

private:
GLuint genVertexArrayObject() const;
GLuint genBufferObject() const;

template<typename T> void createAttributes(T& attributes) const;
Mesh::StaticAttributes createStaticAttributes() const;
Mesh::DynamicAttributes createDynamicAttributes() const;

size_t computeNumberOfVertices() const;

template<typename T> std::vector <GLuint> createDynamicAttributesBufferObject(
    const std::vector <std::vector <T> >& attributes) const;
std::vector <GLuint> createVertexDynamicBufferObjects() const;
std::map <StaticAttributeType, GLuint> createVertexStaticBufferObjects() const;

template<typename T> std::shared_ptr <GLuint> initializeVBO(const std::vector <T> attributeData) const;
template<typename T> void upsertUniforms(const std::map <std::string, T> uniformsData);

const ShaderProgram& _shaderProgram;
const GLuint _vertexArrayObject;
const vecCstMesh_sptr _meshes;
const size_t _numberOfVertices;
const Mesh::StaticAttributes _staticAttributes;
Mesh::DynamicAttributes _dynamicAttributes;
const std::map <StaticAttributeType, GLuint> _vertexStaticBufferObjects;
const std::vector <GLuint> _vertexDynamicBufferObjects;

GLuint _elementBufferObject;
uniformVariable <glm::mat4> _uniformMatrix4;
uniformVariable <glm::vec4> _uniformVec4;
uniformVariable <glm::vec3> _uniformVec3;
uniformVariable <glm::vec2> _uniformVec2;
uniformVariable <GLfloat> _uniformFloat;
uniformVariable <GLuint> _uniformTextures;

};


#endif /* RENDER_PASS_H */
