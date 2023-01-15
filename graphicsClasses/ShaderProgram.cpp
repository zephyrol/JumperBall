/*
 * File: ShaderProgram.cpp
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 17:13
 */

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(
    CstShader_uptr &&vertexShader,
    CstShader_uptr &&fragmentShader,
    GLuint shaderProgramHandle,
    std::vector<UniformLocation> &&uniformLocations
) :
    _shaderProgramHandle(shaderProgramHandle),
    _vertexShader(std::move(vertexShader)),
    _fragmentShader(std::move(fragmentShader)),
    _uniformLocations(std::move(uniformLocations)) {

}

GLuint ShaderProgram::getHandle() const {
    return _shaderProgramHandle;
}

void ShaderProgram::freeGPUMemory() const {
    _vertexShader->freeGPUMemory();
    _fragmentShader->freeGPUMemory();
    glDeleteProgram(_shaderProgramHandle);
}

CstShaderProgram_sptr ShaderProgram::createInstance(
    const JBTypes::FileContent &fileContent,
    const std::string &vs,
    const std::string &fs,
    const std::vector<std::string> &uniformNames,
    const std::vector<std::string> &defines,
    const std::map<std::string, glm::vec2> &constVec2s
) {
    const auto shaderProgramHandle = glCreateProgram();
    if (shaderProgramHandle == 0) {
        std::cerr << "Error during creation of the shader program ..."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    auto vertexShader = Shader::createVertexShader(fileContent, vs, defines, {});
    auto fragmentShader = Shader::createFragmentShader(fileContent, fs, defines, constVec2s);

    glAttachShader(shaderProgramHandle, vertexShader->getHandle());
    glAttachShader(shaderProgramHandle, fragmentShader->getHandle());

    glLinkProgram(shaderProgramHandle);

    verifyLinkStatus(shaderProgramHandle);

    glDetachShader(shaderProgramHandle, vertexShader->getHandle());
    glDetachShader(shaderProgramHandle, fragmentShader->getHandle());
    glDeleteShader(vertexShader->getHandle());
    glDeleteShader(fragmentShader->getHandle());

    std::vector<ShaderProgram::UniformLocation> uniformLocations;
    for (const std::string &name: uniformNames) {
        const auto location = glGetUniformLocation(shaderProgramHandle, name.c_str());
        uniformLocations.push_back({name, location});
    }

    return std::make_shared<const ShaderProgram>(
        std::move(vertexShader),
        std::move(fragmentShader),
        shaderProgramHandle,
        std::move(uniformLocations)
    );
}

void ShaderProgram::use() const {
    glUseProgram(_shaderProgramHandle);
}

void ShaderProgram::verifyLinkStatus(GLuint shaderProgramHandle) {

    GLint status;
    glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        std::cerr << "Shader program linking failed : " << std::endl;
        GLint logLength;
        glGetProgramiv(shaderProgramHandle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            std::string log(logLength, ' ');
            GLsizei message;
            glGetProgramInfoLog(shaderProgramHandle, logLength, &message, &log[0]);
            std::cerr << "Error log : " << std::endl << log;
        }
        exit(EXIT_FAILURE);
    }
}

void ShaderProgram::bindUniform(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::bindUniform(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(getLocation(name), 1, &value[0]);
}

void ShaderProgram::bindUniform(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(getLocation(name), 1, &value[0]);
}

void ShaderProgram::bindUniform(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(getLocation(name), 1, &value[0]);
}

void ShaderProgram::bindUniform(const std::string &name, const GLfloat &value) const {
    glUniform1fv(getLocation(name), 1, &value);
}

void ShaderProgram::bindUniform(const std::string &name, const bool &value) const {
    glUniform1i(getLocation(name), value);
}


void ShaderProgram::bindUniform(const std::string &name, const int &value) const {
    glUniform1i(getLocation(name), value);
}

void ShaderProgram::bindUniform(const std::string &name, const std::vector<int> &value) const {
    const auto size = static_cast <GLsizei>(value.size());
    glUniform1iv(getLocation(name), size, value.data());
}

void ShaderProgram::bindUniform(const std::string &name, const std::vector<float> &value) const {
    const auto size = static_cast <GLsizei>(value.size());
    glUniform1fv(getLocation(name), size, value.data());
}

void ShaderProgram::bindUniformTextureIndex(const std::string &name, int textureNumber) const {
    glUniform1i(getLocation(name), textureNumber);
}

void ShaderProgram::setActiveTexture(int textureNumber) {
    glActiveTexture(GL_TEXTURE0 + textureNumber);
}

void ShaderProgram::bindTexture(GLuint textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLint ShaderProgram::getLocation(const std::string &uniformName) const {
    // Usually, there is a few of uniforms, so sequential research is faster than binary search
    // or ordered map using.
    return std::find_if(
        _uniformLocations.begin(),
        _uniformLocations.end(),
        [&uniformName](const UniformLocation& uniformLocation) {
            return uniformLocation.name == uniformName;
        })->location;
}
