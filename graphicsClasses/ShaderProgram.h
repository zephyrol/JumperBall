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
    ShaderProgram(CstShader_uptr &&vertexShader, CstShader_uptr &&fragmentShader);

    ShaderProgram(const ShaderProgram &shaderProgram) = delete;

    ShaderProgram &operator=(const ShaderProgram &shaderProgram) = delete;

    ~ShaderProgram() = default;

    GLuint getHandle() const;

    void freeGPUMemory() const;

    static CstShaderProgram_sptr createShaderProgram(
        const JBTypes::FileContent &fileContent,
        const std::string &vs,
        const std::string &fs,
        const std::vector<std::string> &defines = {},
        const std::map<std::string, glm::vec2> &constVec2s = {}
    );

    void use() const;

    void bindUniform(const std::string &name, const glm::mat4 &value) const;

    void bindUniform(const std::string &name, const glm::vec4 &value) const;

    void bindUniform(const std::string &name, const glm::vec3 &value) const;

    void bindUniform(const std::string &name, const glm::vec2 &value) const;

    void bindUniform(const std::string &name, const GLfloat &value) const;

    void bindUniform(const std::string &name, const bool &value) const;

    void bindUniform(const std::string &name, const int &value) const;

    void bindUniform(const std::string &name, const std::vector<int> &value) const;

    void bindUniform(const std::string &name, const std::vector<float> &value) const;

    void bindUniformTextureIndex(const std::string &name, int textureNumber) const;

    static void setActiveTexture(int textureNumber);

    static void bindTexture(GLuint textureID);

private:
    const GLuint _shaderProgramHandle;
    const CstShader_uptr _vertexShader;
    const CstShader_uptr _fragmentShader;

// TODO: define the list of uniform in constructor
    mutable std::unordered_map<std::string, GLint> _cacheUniformLocation;

    GLuint fillCacheAndGet(const std::string &name) const;

    void verifyLinkStatus() const;

};

#endif /* SHADERPROGRAM_H */
