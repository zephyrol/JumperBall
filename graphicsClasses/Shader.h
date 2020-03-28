/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shader.h
 * Author: Morgenthaler S
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

    //--CONSTRUCTORS & DESTRUCTORS--//
    Shader            ( const GLenum& shaderType, 
                        const std::string& shaderFilename);
    ~Shader           ();

    //-------CONST METHODS--------//
    GLuint            getHandle() const;

    //------DELETED METHODS-------//
    Shader&           operator = (const Shader& shader)                = delete;
    Shader            (const Shader& orig)                             = delete; 


private:

    //--------ATTRIBUTES-----------//
    const GLuint      _shaderHandle;
    const GLenum      _shaderType;
    const std::string _shaderFilename;
    const std::string _shaderCode;

    //-------CONST METHODS--------//
    void              verifyCompileStatus()                               const;

};

#endif /* SHADER_H */

