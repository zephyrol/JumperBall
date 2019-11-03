/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderProgram.cpp
 * Author: seb
 * 
 * Created on 3 novembre 2019, 17:13
 */

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram( const Shader& vertexShader, 
                              const Shader& fragmentShader) :
                _shaderProgramHandle(glCreateProgram()),
                _vertexShader(vertexShader),
                _fragmentShader(fragmentShader)

{
    if (_shaderProgramHandle == 0 ) {
        std::cerr << "Error during creation of the shader program ..." 
                << std::endl;
        exit(EXIT_FAILURE);
    }

    glAttachShader(_shaderProgramHandle,_vertexShader.getHandle());
    glAttachShader(_shaderProgramHandle,_fragmentShader.getHandle());
    
    glLinkProgram(_shaderProgramHandle);

    verifyLinkStatus();


}

GLuint ShaderProgram::getHandle() const {
    return _shaderProgramHandle;
}

void ShaderProgram::use() const {
    glUseProgram( _shaderProgramHandle );
}

void ShaderProgram::verifyLinkStatus() {
    
    GLint status ;
    glGetProgramiv(_shaderProgramHandle, GL_LINK_STATUS, &status);

    if (status== GL_FALSE) {
        std::cerr << "Shader program linking failed : " << std::endl;
        GLint logLength;
        glGetProgramiv(_shaderProgramHandle, GL_INFO_LOG_LENGTH, &logLength);
        if ( logLength > 0 ) {
            std::string log (logLength, ' ');
            GLsizei message;
            glGetProgramInfoLog (_shaderProgramHandle, logLength, 
                    &message, &log[0]);
            std::cerr << "Error log : " << std::endl << log ;
        }
    } 
    else {
        std::cout << "Shader Program successfully linked" << std::endl;
    }
}


ShaderProgram::~ShaderProgram() {
    glDetachShader(_shaderProgramHandle,_vertexShader.getHandle());
    glDetachShader(_shaderProgramHandle,_fragmentShader.getHandle());
}

