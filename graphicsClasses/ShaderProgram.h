/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderProgram.h
 * Author: Morgenthaler S
 *
 * Created on 3 novembre 2019, 17:13
 */

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include "Shader.h"

class ShaderProgram {

public:
    //--CONSTRUCTORS & DESTRUCTORS--//
    ShaderProgram   (const Shader& vertexShader, const Shader& fragmentShader);


    //------DELETED METHODS-------//
    ShaderProgram&  operator = (const ShaderProgram& shaderProgram)    = delete;
    ShaderProgram   (const ShaderProgram& orig)                        = delete; 


    //-------CONST METHODS--------//
    GLuint          getHandle()                                           const;
    void            use()                                                 const;


private:
    //--------ATTRIBUTES-----------//
    const GLuint    _shaderProgramHandle; 
    const Shader&   _vertexShader; 
    const Shader&   _fragmentShader; 


    //-------CONST METHODS--------//
    void            verifyLinkStatus()                                    const;

};

#endif /* SHADERPROGRAM_H */

