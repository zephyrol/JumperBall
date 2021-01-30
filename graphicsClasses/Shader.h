/*
 * File: Shader.h
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 16:10
 */

#ifndef SHADER_H
#define SHADER_H
#include "Utility.h"

class Shader {

public:

// --CONSTRUCTORS & DESTRUCTORS--//
Shader(const GLenum& shaderType,
       const std::string& shaderFilename);

Shader(const Shader& shader) = delete;
Shader& operator= (const Shader& shader) = delete;

Shader(Shader&& shader) = default;
Shader& operator= (Shader&& shader) = default;

~Shader() = default;

// -------CONST METHODS--------//
GLuint getHandle() const;
GLenum getShaderType() const;

private:

// --------ATTRIBUTES-----------//
GLuint _shaderHandle;
GLenum _shaderType;
std::string _shaderFilename;
std::string _shaderCode;

// -------CONST METHODS--------//
void verifyCompileStatus() const;

};

#endif /* SHADER_H */
