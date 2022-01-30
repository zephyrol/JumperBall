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

Shader(
    const GLenum& shaderType,
    const std::string& shaderFilename,
    const std::vector<std::string>& defines
);

Shader(const Shader& shader) = delete;
Shader& operator= (const Shader& shader) = delete;

~Shader() = default;

GLuint getHandle() const;
GLenum getShaderType() const;
void freeGPUMemory() const;

static CstShader_uptr createVertexShader(
    const std::string& shaderName,
    const std::vector<std::string>& defines
);

static CstShader_uptr createFragmentShader(
    const std::string& shaderName,
    const std::vector<std::string>& defines
);

static const std::string shadersDir;

private:

    static std::string cleanDefines(const std::string& shaderCode, const std::vector<std::string>& defines);
const GLuint _shaderHandle;
const GLenum _shaderType;
const std::string _shaderFilename;
const std::string _shaderCode;

void verifyCompileStatus(const std::string& shaderCode) const;

};

#endif /* SHADER_H */
