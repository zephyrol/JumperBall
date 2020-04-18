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
#include "Utility.h"

class Shader {

public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Shader            ( const GLenum& shaderType, 
                        const std::string& shaderFilename);

    Shader&           operator = (const Shader& shader)                = delete;
    Shader            (const Shader& shader)                           = delete; 

    Shader&           operator = (Shader&& shader)                    = default;
    Shader            (Shader&& shader)                               = default;

    ~Shader           ()                                              = default;

    //-------CONST METHODS--------//
    GLuint            getHandle() const;

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

