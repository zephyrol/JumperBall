/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shader.h
 * Author: seb
 *
 * Created on 3 novembre 2019, 16:10
 */

#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Utility.h"

class Shader {

public:
    Shader(const GLenum& shaderType, const std::string& shaderFilename);

    Shader& operator = (const Shader& shader) = delete;
    Shader(const Shader& orig) = delete ; 

    GLuint getHandle() const;
    virtual ~Shader();

private:

    const GLuint      _shaderHandle;
    const GLenum      _shaderType;
    const std::string _shaderFilename;
    const std::string _shaderCode;

    void verifyCompileStatus();

};

#endif /* SHADER_H */

