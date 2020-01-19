/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shader.cpp
 * Author: seb
 * 
 * Created on 3 novembre 2019, 16:10
 */

#include "Shader.h"

Shader::Shader(const GLenum& shaderType, const std::string& shaderFilename) :
              _shaderHandle( glCreateShader(shaderType) ),
              _shaderType(shaderType),
              _shaderFilename(shaderFilename),
              _shaderCode(Utility::readFileSrc(shaderFilename))
{
    if (_shaderHandle == 0 ) {
        std::cerr << "Error during creation of the shader ..." << std::endl;
        exit(EXIT_FAILURE);
    }

    constexpr GLsizei numberOfStrings = 1;
    const GLchar* const glCode = _shaderCode.c_str();

    glShaderSource(_shaderHandle, numberOfStrings, &glCode, nullptr);
    glCompileShader(_shaderHandle);
    verifyCompileStatus();    

}

void Shader::verifyCompileStatus() const {
    GLint status;
    glGetShaderiv(_shaderHandle, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        std::cerr << "Shader compilation failed : " << _shaderFilename 
                << std::endl;
        GLint logLength;
        glGetShaderiv(_shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
        if ( logLength > 0 ) {
            std::string log (logLength, ' ');
            GLsizei message;
            glGetShaderInfoLog (_shaderHandle, logLength, &message, &log[0]);
            std::cerr << "Error log : " << std::endl << log ;
        }
    } 
    else {
        std::cout << "Shader successfully compiled: " << _shaderFilename 
                << std::endl;
    }
}

GLuint Shader::getHandle() const {
    return _shaderHandle;
}


Shader::~Shader() {
}

