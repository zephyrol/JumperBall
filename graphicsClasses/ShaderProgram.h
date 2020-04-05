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

    ShaderProgram&  operator = (const ShaderProgram& shaderProgram)    = delete;
    ShaderProgram   (const ShaderProgram& shaderProgram)               = delete; 

    ShaderProgram&  operator = (ShaderProgram&& shaderProgram)        = default;
    ShaderProgram   (ShaderProgram&& shaderProgram)                   = default; 

    ~ShaderProgram  ()                                                = default;


    //-------CONST METHODS--------//
    GLuint          getHandle()                                           const;
    void            use()                                                 const;

    void            bindUniform(const std::string&    name,
                                const glm::mat4&      value)              const;

    void            bindUniform(const std::string&    name,
                                const glm::vec4&      value)              const;

    void            bindUniform(const std::string&    name,
                                const glm::vec3&      value)              const;

    void            bindUniform(const std::string&    name,
                                const glm::vec2&      value)              const;

    void            bindUniform(const std::string&    name,
                                const GLfloat&        value)              const;

    void            bindUniform(const std::string&    name,
                                const bool&           value)              const;

    void            bindUniform(const std::string&    name,
                                const int&            value)              const;

    void            bindUniform(const std::string&    name,
                                const std::vector<int>& value)            const;

    void            bindUniform(const std::string&    name,
                                const std::vector<float>& value)          const;

    void            bindUniformTexture
                                (const std::string&    name,
                                const int&             value)             const;

private:
    //--------ATTRIBUTES-----------//
    const GLuint    _shaderProgramHandle; 
    const Shader&   _vertexShader; 
    const Shader&   _fragmentShader; 


    //-------CONST METHODS--------//
    void            verifyLinkStatus()                                    const;

};

#endif /* SHADERPROGRAM_H */

