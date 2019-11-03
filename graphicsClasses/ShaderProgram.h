/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderProgram.h
 * Author: seb
 *
 * Created on 3 novembre 2019, 17:13
 */

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include "Shader.h"

class ShaderProgram {

public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);

    ShaderProgram& operator = (const ShaderProgram& shaderProgram) = delete;
    ShaderProgram(const ShaderProgram& orig) = delete ; 

    GLuint getHandle() const;

    void use() const;

    virtual ~ShaderProgram();

private:
    const GLuint _shaderProgramHandle; 
    const Shader& _vertexShader; 
    const Shader& _fragmentShader; 

    void verifyLinkStatus();

};

#endif /* SHADERPROGRAM_H */

