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

    //--CONSTRUCTORS & DESTRUCTORS--//
    UniformBlock                      (  const std::vector<std::string>& 
                                                                variablesNames);
    

    UniformBlock                      ( const UniformBlock& 
                                                uniformBlock);
    
    virtual ~UniformBlock             ();                                    
    

    //---------CONSTANTS------------//
    static constexpr size_t           sizeVec3f           = 3 * sizeof(GLfloat);


    //------------TYPES------------//
    struct  variablesNamesInfo        { const char *const * names;
                                        size_t number; };


    //----------METHODS------------//
    UniformBlock&                     operator=( const UniformBlock& 
                                                        uniformBlock);
    virtual void                      bind( const std::string& name,
                                            const ShaderProgram& sp)        = 0;


    //--------STATIC METHODS-------//
    static struct variablesNamesInfo  getStringsStoredLinearly( 
                                                const std::vector<std::string>&
                                                  strNames);
    static struct variablesNamesInfo  copyVariablesNamesInfo( 
                                                const struct variablesNamesInfo& 
                                                  varNamesInfo);
    
protected:

    //--------ATTRIBUTES-----------//
    std::vector<GLchar>               _dataBuffer;


    //-------CONST METHODS----------//
    const struct variablesNamesInfo&  variablesNames()                    const;
    const std::vector<GLuint>&        variablesIndices()                  const;
    const std::vector<GLint>&         variablesOffsets()                  const;
    const std::vector<GLchar>&        dataBuffer()                        const;
    GLint                             blockSize()                         const;
    GLuint                            uboHandle()                         const;


    //----------METHODS------------//
    void                              configureDataBuffer(
                                                const ShaderProgram& sp,
                                                const std::string& name); 


private:

    //--------ATTRIBUTES-----------//
    const struct variablesNamesInfo   _variablesNames;
    std::vector<GLuint>               _variablesIndices;
    std::vector<GLint>                _variablesOffsets;
    GLint                             _blockSize;
    GLuint                            _uboHandle;

    //----------METHODS------------//
    void                              deleteVariablesNamesInfo();
};

#endif /* UNIFORMBLOCK_H */

