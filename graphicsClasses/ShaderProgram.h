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

using ShaderProgram_sptr = std::shared_ptr<ShaderProgram>;
using CstShaderProgram_sptr = std::shared_ptr<const ShaderProgram>;
using vecCstShaderProgram_sptr = std::vector<CstShaderProgram_sptr>;

class ShaderProgram {

public:

    ShaderProgram(
        CstShader_uptr &&vertexShader,
        CstShader_uptr &&fragmentShader,
        GLuint shaderProgramHandle
    );

    ShaderProgram(const ShaderProgram &shaderProgram) = delete;

    ShaderProgram &operator=(const ShaderProgram &shaderProgram) = delete;

    ~ShaderProgram() = default;

    GLuint getHandle() const;

    void freeGPUMemory() const;

    static CstShaderProgram_sptr createInstance(
        const JBTypes::FileContent &fileContent,
        const std::string &vs,
        const std::string &fs,
        const std::vector<std::string> &defines = {},
        const std::map<std::string, glm::vec2> &constVec2s = {}
    );

    void use() const;

private:
    const GLuint _shaderProgramHandle;
    const CstShader_uptr _vertexShader;
    const CstShader_uptr _fragmentShader;

    static void verifyLinkStatus(GLuint shaderProgramHandle);

};

#endif /* SHADERPROGRAM_H */
