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
void upsertUniformTexture(const std::string& name, const GLuint value);

private:
GLuint genVertexArrayObject() const;
GLuint genBufferObject() const;
Mesh::StaticAttributes createStaticAttributes() const;
std::map <StaticAttributeType, GLuint> createVertexBufferObjects() const;

template<typename T> std::shared_ptr <GLuint> initializeVBO(const std::vector <T> staticAttributeData) const;

const ShaderProgram& _shaderProgram;
const GLuint _vertexArrayObject;
const vecCstMesh_sptr _meshes;
const Mesh::StaticAttributes _staticAttributes;
const std::map <StaticAttributeType, GLuint> _vertexBufferObjects;

GLuint _elementBufferObject;

uniformVariable <glm::mat4> _uniformMatrix4;
uniformVariable <glm::vec4> _uniformVec4;
uniformVariable <glm::vec3> _uniformVec3;
uniformVariable <glm::vec2> _uniformVec2;
uniformVariable <GLfloat> _uniformFloat;
uniformVariable <GLuint> _uniformTextures;

};


#endif /* RENDER_PASS_H */
