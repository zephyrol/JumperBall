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
    GLuint shaderProgramHandle
) :
    _shaderProgramHandle(shaderProgramHandle),
    _vertexShader(std::move(vertexShader)),
    _fragmentShader(std::move(fragmentShader)) {

}

GLuint ShaderProgram::getHandle() const {
    return _shaderProgramHandle;
}

void ShaderProgram::freeGPUMemory() {
    _vertexShader->freeGPUMemory();
    _fragmentShader->freeGPUMemory();
    glDeleteProgram(_shaderProgramHandle);
}

ShaderProgram_sptr ShaderProgram::createInstance(
    const JBTypes::FileContent &fileContent,
    const std::string &vs,
    const std::string &fs,
    const std::vector<std::string> &defines,
    const std::vector<std::pair<std::string, GLshort>> &constShorts,
    const std::vector<std::pair<std::string, GLfloat>> &constFloats,
    const std::vector<std::pair<std::string, glm::vec2>> &constVec2s
) {
    const auto shaderProgramHandle = glCreateProgram();
    if (shaderProgramHandle == 0) {
        std::cerr << "Error during creation of the shader program ..."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    auto vertexShader = Shader::createVertexShader(
        fileContent,
        vs,
        defines,
        constShorts,
        constFloats,
        constVec2s
    );
    auto fragmentShader = Shader::createFragmentShader(
        fileContent,
        fs,
        defines,
        constShorts,
        constFloats,
        constVec2s
    );

    glAttachShader(shaderProgramHandle, vertexShader->getHandle());
    glAttachShader(shaderProgramHandle, fragmentShader->getHandle());

    glLinkProgram(shaderProgramHandle);

    verifyLinkStatus(shaderProgramHandle);

    glDetachShader(shaderProgramHandle, vertexShader->getHandle());
    glDetachShader(shaderProgramHandle, fragmentShader->getHandle());
    glDeleteShader(vertexShader->getHandle());
    glDeleteShader(fragmentShader->getHandle());

    return std::make_shared<ShaderProgram>(
        std::move(vertexShader),
        std::move(fragmentShader),
        shaderProgramHandle
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

GLint ShaderProgram::getUniformLocation(const std::string &uniformName) const {
    const auto location = glGetUniformLocation(
        _shaderProgramHandle,
        uniformName.c_str()
    );
    return location;
}

void ShaderProgram::setTextureIndex(const std::string &textureName, GLint index) {
    const auto location = getUniformLocation(textureName);
    glUniform1i(location, index);
}

void ShaderProgram::setUniformArrayVec2(
    const std::string &uniformArrayName,
    const std::vector<GLfloat> &vec2sData
) {
    const auto location = getUniformLocation(uniformArrayName);
    glUniform2fv(location, static_cast<GLint>(vec2sData.size()) / 2, vec2sData.data());
}

void ShaderProgram::setUniformArrayVec4(
    const std::string &uniformArrayName,
    const std::vector<GLfloat> &vec4sData
) {
    const auto location = getUniformLocation(uniformArrayName);
    glUniform4fv(location, static_cast<GLint>(vec4sData.size()) / 4, vec4sData.data());
}
