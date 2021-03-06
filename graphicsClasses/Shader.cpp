/*
 * File: Shader.cpp
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 16:10
 */

#include "Shader.h"

Shader::Shader(const GLenum& shaderType, const std::string& shaderFilename):
    _shaderHandle(glCreateShader(shaderType)),
    _shaderType(shaderType),
    _shaderFilename(shaderFilename),
    _shaderCode(Utility::readFileSrc(shaderFilename)) {
    if (_shaderHandle == 0) {
        std::cerr << "Error during creation of the shader ..." << std::endl;
        exit(EXIT_FAILURE);
    }

    constexpr GLsizei numberOfStrings = 1;
    const GLchar*const glCode = _shaderCode.c_str();

    glShaderSource(_shaderHandle, numberOfStrings, &glCode, nullptr);
    glCompileShader(_shaderHandle);
    verifyCompileStatus();
}

void Shader::verifyCompileStatus() const {

    GLint status;
    glGetShaderiv(_shaderHandle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        std::cerr << "Shader compilation failed : " << _shaderFilename << std::endl;
        GLint logLength;
        glGetShaderiv(_shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            std::string log(logLength, ' ');
            GLsizei message;
            glGetShaderInfoLog(_shaderHandle, logLength, &message, &log[0]);
            std::cerr << "Error log : " << std::endl << log;
        }
    }
}

GLuint Shader::getHandle() const {
    return _shaderHandle;
}

GLenum Shader::getShaderType() const {
    return _shaderType;
}

void Shader::freeGPUMemory() const {
    glDeleteShader(_shaderHandle);
}

const std::string Shader::shadersDir = "shaders/";
CstShader_uptr Shader::createVertexShader (const std::string& shaderName) {
    return std::unique_ptr <const Shader>(new Shader(GL_VERTEX_SHADER, shadersDir + shaderName));
}
CstShader_uptr Shader::createFragmentShader (const std::string& shaderName) {
    return std::unique_ptr <const Shader>(new Shader(GL_FRAGMENT_SHADER, shadersDir + shaderName));
}
