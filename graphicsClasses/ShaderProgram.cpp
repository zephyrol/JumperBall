/*
 * File: ShaderProgram.cpp
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 17:13
 */

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(CstShader_uptr &&vertexShader, CstShader_uptr &&fragmentShader) :
    _shaderProgramHandle(glCreateProgram()),
    _vertexShader(std::move(vertexShader)),
    _fragmentShader(std::move(fragmentShader)),
    _cacheUniformLocation() {
    if (_shaderProgramHandle == 0) {
        std::cerr << "Error during creation of the shader program ..."
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    glAttachShader(_shaderProgramHandle, _vertexShader->getHandle());
    glAttachShader(_shaderProgramHandle, _fragmentShader->getHandle());

    glLinkProgram(_shaderProgramHandle);

    verifyLinkStatus();

    glDetachShader(_shaderProgramHandle, _vertexShader->getHandle());
    glDetachShader(_shaderProgramHandle, _fragmentShader->getHandle());
    glDeleteShader(_vertexShader->getHandle());
    glDeleteShader(_fragmentShader->getHandle());

}

GLuint ShaderProgram::getHandle() const {
    return _shaderProgramHandle;
}

void ShaderProgram::freeGPUMemory() const {
    _vertexShader->freeGPUMemory();
    _fragmentShader->freeGPUMemory();
    glDeleteProgram(_shaderProgramHandle);
}

CstShaderProgram_sptr ShaderProgram::createShaderProgram(
    const JBTypes::FileContent &fileContent,
    const std::string &vs,
    const std::string &fs,
    const std::vector<std::string> &defines,
    const std::map<std::string, glm::vec2> &constVec2s
) {
    return std::make_shared<const ShaderProgram>(
        Shader::createVertexShader(fileContent, vs, defines, {}),
        Shader::createFragmentShader(fileContent, fs, defines, constVec2s)
    );
}

void ShaderProgram::use() const {
    glUseProgram(_shaderProgramHandle);
}

void ShaderProgram::verifyLinkStatus() const {

    GLint status;
    glGetProgramiv(_shaderProgramHandle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        std::cerr << "Shader program linking failed : " << std::endl;
        GLint logLength;
        glGetProgramiv(_shaderProgramHandle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            std::string log(logLength, ' ');
            GLsizei message;
            glGetProgramInfoLog(_shaderProgramHandle, logLength, &message, &log[0]);
            std::cerr << "Error log : " << std::endl << log;
        }
        exit(EXIT_FAILURE);
    }
}

void ShaderProgram::bindUniform(const std::string &name, const glm::mat4 &value) const {

    const auto uniformVariableID = fillCacheAndGet(name);
    glUniformMatrix4fv(uniformVariableID, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::bindUniform(const std::string &name, const glm::vec4 &value) const {

    const auto uniformVariableID = fillCacheAndGet(name);
    glUniform4fv(uniformVariableID, 1, &value[0]);
}

void ShaderProgram::bindUniform(const std::string &name, const glm::vec3 &value) const {

    const auto uniformVariableID = fillCacheAndGet(name);
    glUniform3fv(uniformVariableID, 1, &value[0]);
}

void ShaderProgram::bindUniform(const std::string &name, const glm::vec2 &value) const {

    const auto uniformVariableID = fillCacheAndGet(name);
    glUniform2fv(uniformVariableID, 1, &value[0]);
}

void ShaderProgram::bindUniform(const std::string &name, const GLfloat &value) const {

    const auto uniformVariableID = fillCacheAndGet(name);
    glUniform1fv(uniformVariableID, 1, &value);
}

void ShaderProgram::bindUniform(const std::string &name, const bool &value) const {

    const auto uniformVariableID = fillCacheAndGet(name);
    glUniform1i(uniformVariableID, value);
}


void ShaderProgram::bindUniform(const std::string &name, const int &value) const {

    const auto uniformVariableID = fillCacheAndGet(name);
    glUniform1i(uniformVariableID, value);
}

void ShaderProgram::bindUniform(const std::string &name, const std::vector<int> &value) const {
    const auto uniformVariableID = fillCacheAndGet(name);
    const auto size = static_cast <GLsizei>(value.size());
    glUniform1iv(uniformVariableID, size, value.data());
}

void ShaderProgram::bindUniform(const std::string &name, const std::vector<float> &value) const {
    const auto uniformVariableID = fillCacheAndGet(name);
    const auto size = static_cast <GLsizei>(value.size());
    glUniform1fv(uniformVariableID, size, value.data());
}

void ShaderProgram::bindUniformTextureIndex(const std::string &name, int textureNumber) const {
    const auto uniformVariableID = fillCacheAndGet(name);
    glUniform1i(uniformVariableID, textureNumber);
}

void ShaderProgram::setActiveTexture(int textureNumber) {
    glActiveTexture(GL_TEXTURE0 + textureNumber);
}

void ShaderProgram::bindTexture(GLuint textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLint ShaderProgram::fillCacheAndGet(const std::string &name) const {
    const auto uniformIterator = _cacheUniformLocation.find(name);
    if (uniformIterator == _cacheUniformLocation.end()) {
        const auto uniformLocation = glGetUniformLocation(getHandle(), name.c_str());
        _cacheUniformLocation[name] = uniformLocation;
        return uniformLocation;
    }
    return uniformIterator->second;
}

