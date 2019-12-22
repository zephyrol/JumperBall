/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uniformBlock.h
 * Author: morgenthaler
 *
 * Created on 22 décembre 2019, 12:22
 */

#ifndef UNIFORMBLOCK_H
#define UNIFORMBLOCK_H
#include <Types.h>
#include <iostream>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Shader.h"
#include "../ShaderProgram.h"

class uniformBlock {
public:
    uniformBlock                    ( const ShaderProgram& sp, 
                                      const std::string name,
                                      const std::vector<std::string>& 
                                              variablesNames
                                    );
    virtual                         ~uniformBlock()                         = 0;
    
private:

    const GLuint                    _blockIndex;
    GLsizei                         _blockSize;
    std::vector<GLubyte>            _dataInsideBlock;
    const std::vector<std::string>  _variablesNames;
    std::vector<GLuint>             _variablesIndices;
    GLuint                          _uboHandle;

};

#endif /* UNIFORMBLOCK_H */

