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

RenderPass(const ShaderProgram& shaderProgram, const vecMesh_sptr& meshes);
RenderPass(const RenderPass& renderPass) = delete;
RenderPass& operator= (const RenderPass& renderPass) = delete;
RenderPass(RenderPass&& renderPass) = default;
RenderPass& operator= (RenderPass&& renderPass) = default;

template<typename T> using UniformVariable = std::map <std::string, T>;

void render() const;

enum class ShapeVertexAttributeType { Positions, Normals, Colors, UvCoords, Indices };

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
GeometricShape::ShapeVertexAttributes createShapeVertexAttributes() const;
Mesh::StateVertexAttributes createStateVertexAttributes() const;

size_t computeNumberOfVertices() const;

void bindUniforms() const;
template<typename T> void bindUniforms(UniformVariable <T> uniforms) const;

template<typename T> std::vector <GLuint> createStateVertexAttributesBufferObject(
    const std::vector <std::vector <T> >& attributes) const;
std::vector <GLuint> createVertexDynamicBufferObjects() const;
std::map <ShapeVertexAttributeType, GLuint> createVertexStaticBufferObjects() const;

template<typename T> std::shared_ptr <GLuint> initializeBO(
    const std::vector <T>& attributeData,
    GLenum target
    ) const;
template<typename T> std::shared_ptr <GLuint> initializeVBO(const std::vector <T>& attributeData) const;
std::shared_ptr <GLuint> initializeEBO(const std::vector <GLushort>& indicesData) const;

template<typename T> void upsertUniforms(const std::map <std::string, T>& uniformsData);

const ShaderProgram& _shaderProgram;
const GLuint _vertexArrayObject;
const vecMesh_sptr _meshes;
const size_t _numberOfVertices;
const GeometricShape::ShapeVertexAttributes _shapeVertexAttributes;
Mesh::StateVertexAttributes _stateVertexAttributes;
const std::map <ShapeVertexAttributeType, GLuint> _vertexStaticBufferObjects;
const std::vector <GLuint> _vertexDynamicBufferObjects;

UniformVariable <glm::mat4> _uniformMatrix4;
UniformVariable <glm::vec4> _uniformVec4;
UniformVariable <glm::vec3> _uniformVec3;
UniformVariable <glm::vec2> _uniformVec2;
UniformVariable <GLfloat> _uniformFloat;
UniformVariable <GLuint> _uniformTextures;

};


#endif /* RENDER_PASS_H */
