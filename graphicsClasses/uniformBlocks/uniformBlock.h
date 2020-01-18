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

class UniformBlock {
public:
    UniformBlock                      ( //const ShaderProgram& sp, 
                                        const std::vector<std::string>& 
                                                variablesNames
                                      );
    

    UniformBlock                     ( const UniformBlock& 
                                                uniformBlock)          = delete;
    UniformBlock&                     operator=( const UniformBlock& 
                                                        uniformBlock)  = delete;
    
    virtual ~UniformBlock             ()                                    = 0;
    
    virtual void                      bind( const std::string& name,
                                            const ShaderProgram& sp)        = 0; 


    static constexpr size_t           sizeVec3f           = 3 * sizeof(GLfloat);
    
protected:


    struct  variablesNames            { const char* const * const names;
                                        const size_t number; };

    //const GLuint                    _blockIndex;
    //GLsizei                           _blockSize;
    //std::vector<GLubyte>              _dataInsideBlock;
    //std::vector<std::string>          _variablesNames;
    const struct variablesNames       _variablesNames;
    /*std::vector<GLuint>               _variablesIndices;
    std::vector<GLint>                _variablesOffset;*/
    GLuint                            _uboHandle;


    std::pair<GLint,std::vector<GLint> >
                                      configureDataBuffer(
                                                const ShaderProgram& sp,
                                                const std::string& name)  const; 

private:
    
  struct variablesNames               getStringsStoredLinearly( 
                                                const std::vector<std::string>&
                                                strNames);

};

#endif /* UNIFORMBLOCK_H */

