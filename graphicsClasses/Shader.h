/*
 * File: Shader.h
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 16:10
 */

#ifndef SHADER_H
#define SHADER_H
#include "Utility.h"

class Shader;
using Shader_uptr = std::unique_ptr <Shader>;
using CstShader_uptr = std::unique_ptr <const Shader>;

class Shader {

public:

// --CONSTRUCTORS & DESTRUCTORS--//
Shader(const GLenum& shaderType,
       const std::string& shaderFilename);

Shader(const Shader& shader) = delete;
Shader& operator= (const Shader& shader) = delete;

~Shader() = default;

// -------CONST METHODS--------//
GLuint getHandle() const;
GLenum getShaderType() const;

static CstShader_uptr createVertexShader(const std::string& shaderName);
static CstShader_uptr createFragmentShader(const std::string& shaderName);

static const std::string shadersDir;

private:

// --------ATTRIBUTES-----------//
const GLuint _shaderHandle;
const GLenum _shaderType;
const std::string _shaderFilename;
const std::string _shaderCode;

// -------CONST METHODS--------//
void verifyCompileStatus() const;

};

#endif /* SHADER_H */
