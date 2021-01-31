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
#include "mesh/BallMesh.h"
#include "mesh/MapMesh.h"
#include "mesh/StarMesh.h"
#include "mesh/QuadMesh.h"
#include "Camera.h"
#include "uniformBlocks/uniformLight.h"
#include "animations/BallAnimation.h"
#include ""

class RenderPass {

public:

RenderPass(const ShaderProgram& shaderProgram);
RenderPass(const RenderPass& renderPass) = delete;
RenderPass& operator= (const RenderPass& renderPass) = delete;
RenderPass(RenderPass&& renderPass) = default;
RenderPass& operator= (RenderPass&& renderPass) = default;

template<typename T> using uniformVariable = std::map <std::string, T>;

template<typename T> void upsertVertexAttribute(size_t number, const std::vector <T>& data);

void render() const;
GLuint genVertexArrayObject() const;
GLuint genBufferObject() const;

void update();
void upsertIndices(const std::vector <GLushort>& indices);

void upsertUniform(const std::string& name, const glm::mat4& value);
void upsertUniform(const std::string& name, const glm::vec4& value);
void upsertUniform(const std::string& name, const glm::vec3& value);
void upsertUniform(const std::string& name, const glm::vec2& value);
void upsertUniform(const std::string& name, const bool& value);
void upsertUniformTexture(const std::string& name, const GLuint value);

const ShaderProgram& _shaderProgram;
const GLuint _vertexArrayObject;
std::vector <GLuint> _vertexBufferObjects;

GLuint _elementBufferObject;
std::vector <GLushort> _indices;

uniformVariable <glm::mat4> _uniformMatrix4;
uniformVariable <glm::vec4> _uniformVec4;
uniformVariable <glm::vec3> _uniformVec3;
uniformVariable <glm::vec2> _uniformVec2;
uniformVariable <GLfloat> _uniformFloat;
uniformVariable <bool> _uniformBool;
uniformVariable <GLuint> _uniformTextures;

};


#endif /* RENDER_PASS_H */
