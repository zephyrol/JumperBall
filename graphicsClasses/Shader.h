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

using Shader_uptr = std::unique_ptr<Shader>;
using CstShader_uptr = std::unique_ptr<const Shader>;

class Shader {

public:

    Shader(
        const GLenum &shaderType,
        const JBTypes::FileContent &fileContent,
        const std::string &shaderFilename,
        const std::vector<std::string> &defines,
        const std::vector<std::pair<std::string, GLshort>> &constShorts,
        const std::vector<std::pair<std::string, GLfloat>> &constFloats,
        const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
    );

    Shader(const Shader &shader) = delete;

    Shader &operator=(const Shader &shader) = delete;

    ~Shader() = default;

    GLuint getHandle() const;

    GLenum getShaderType() const;

    void freeGPUMemory() const;

    static CstShader_uptr createVertexShader(
        const JBTypes::FileContent &fileContent,
        const std::string &shaderName,
        const std::vector<std::string> &defines,
        const std::vector<std::pair<std::string, GLshort>> &constShorts,
        const std::vector<std::pair<std::string, GLfloat>> &constFloats,
        const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
    );

    static CstShader_uptr createFragmentShader(
        const JBTypes::FileContent &fileContent,
        const std::string &shaderName,
        const std::vector<std::string> &defines,
        const std::vector<std::pair<std::string, GLshort>> &constShorts,
        const std::vector<std::pair<std::string, GLfloat>> &constFloats,
        const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
    );

private:

    static std::string completeShaderCode(
        const std::string &shaderCode,
        const std::vector<std::string> &defines,
        const std::vector<std::pair<std::string, GLshort>> &constShorts,
        const std::vector<std::pair<std::string, GLfloat>> &constFloats,
        const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
    );

    const GLuint _shaderHandle;
    const GLenum _shaderType;
    const std::string _shaderFilename;
    const std::string _shaderCode;

    void verifyCompileStatus(const std::string &shaderCode) const;

};

#endif /* SHADER_H */
