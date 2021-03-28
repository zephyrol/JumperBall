/*
 * File: ShaderProgram.h
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 17:13
 */

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include "Shader.h"

class ShaderProgram;
using ShaderProgram_uptr = std::unique_ptr <ShaderProgram>;
using CstShaderProgram_uptr = std::unique_ptr <const ShaderProgram>;

class ShaderProgram {

public:
ShaderProgram(CstShader_uptr&& vertexShader, CstShader_uptr&& fragmentShader);

ShaderProgram(const ShaderProgram& shaderProgram) = delete;
ShaderProgram& operator= (const ShaderProgram& shaderProgram) = delete;
~ShaderProgram() = default;

GLuint getHandle() const;
void use() const;

void bindUniform(const std::string& name, const glm::mat4& value) const;
void bindUniform(const std::string& name, const glm::vec4& value) const;
void bindUniform(const std::string& name, const glm::vec3& value) const;
void bindUniform(const std::string& name, const glm::vec2& value) const;
void bindUniform(const std::string& name, const GLfloat& value) const;
void bindUniform(const std::string& name, const bool& value) const;
void bindUniform(const std::string& name, const int& value) const;
void bindUniform(const std::string& name, const std::vector <int>& value) const;
void bindUniform(const std::string& name, const std::vector <float>& value) const;

void bindUniformTexture(const std::string& name, const int& textureNumber, GLuint textureID) const;

private:
const GLuint _shaderProgramHandle;
const CstShader_uptr _vertexShader;
const CstShader_uptr _fragmentShader;

void verifyLinkStatus() const;

};

#endif /* SHADERPROGRAM_H */
