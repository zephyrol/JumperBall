/*
 * File: Shader.cpp
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 16:10
 */

#include "Shader.h"

Shader::Shader(
    const GLenum& shaderType,
    const std::string& shaderFilename,
    const std::vector<std::string>& defines
):
    _shaderHandle(glCreateShader(shaderType)),
    _shaderType(shaderType),
    _shaderFilename(shaderFilename),
    _shaderCode(cleanDefines(Utility::readFileSrc(shaderFilename), defines)) {
    if (_shaderHandle == 0) {
        std::cerr << "Error during creation of the shader ..." << std::endl;
        exit(EXIT_FAILURE);
    }

    constexpr GLsizei numberOfStrings = 1;
    const GLchar*const glCode = _shaderCode.c_str();

    glShaderSource(_shaderHandle, numberOfStrings, &glCode, nullptr);
    glCompileShader(_shaderHandle);
    verifyCompileStatus(_shaderCode);
}

void Shader::verifyCompileStatus(const std::string& shaderCode) const {

    GLint status;
    glGetShaderiv(_shaderHandle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        std::cerr << "Shader compilation failed : " << _shaderFilename << std::endl;
        std::cerr << "Shader code" << std::endl << shaderCode << std::endl;
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

CstShader_uptr Shader::createVertexShader (
    const std::string& shaderName,
    const std::vector<std::string>& defines
) {
    return std::unique_ptr <const Shader>(new Shader(GL_VERTEX_SHADER, shadersDir + shaderName, defines));
}

CstShader_uptr Shader::createFragmentShader (
    const std::string& shaderName,
    const std::vector<std::string>& defines
    ) {
    return std::unique_ptr <const Shader>(new Shader(GL_FRAGMENT_SHADER, shadersDir + shaderName, defines));
}

std::string Shader::cleanDefines(const std::string& shaderCode, const std::vector<std::string> &defines) {
    std::string finalShader = shaderCode;
    const std::string ifdefKey = "#ifdef";
    const std::string endifKey = "#endif";
    for (const auto &define: defines) {
        const std::string defineKey = ifdefKey + "(" + define + ")";
        size_t index;
        while((index = finalShader.find(defineKey)) != std::string::npos) {
            finalShader.replace(index, defineKey.length(),"");
        }
        while((index = finalShader.find(ifdefKey)) != std::string::npos) {
            const auto endifPos = finalShader.find(endifKey, index);
            finalShader.replace(index, endifPos - index,"");
        }
        while((index = finalShader.find(endifKey)) != std::string::npos) {
            finalShader.replace(index, endifKey.length(), "");
        }
    }
    return finalShader;
}

const std::string Shader::shadersDir = "shaders/";
