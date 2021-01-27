
/* 
 * File:   uniformBlock.h
 * Author: Morgenthaler S 
 *
 * Created on 22 décembre 2019, 12:22
 */

#ifndef UNIFORMBLOCK_H
#define UNIFORMBLOCK_H
#include <iostream>
#include <map>
#include <vector>
#include "Shader.h"
#include "ShaderProgram.h"

class UniformBlock {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    UniformBlock                      (const std::string& blockName,
                                       const ShaderProgram &shaderProgram,
                                       const std::vector<std::string>&
                                        variablesNames);

    UniformBlock                      (const UniformBlock& uniformBlock)
                                                                      = delete;
    UniformBlock&                     operator=( const UniformBlock&
                                                        uniformBlock) = delete;

    virtual ~UniformBlock             ();                                    
    

    //---------CONSTANTS------------//
    static constexpr size_t           sizeVec3f           = 3 * sizeof(GLfloat);

    //--------CONST METHODS--------//
    virtual void                      bind()                           const= 0;

    //----------METHODS------------//
    virtual void                      update()                              = 0;

protected:
    //--------CONST METHODS--------//
    const std::vector<GLint>&         variablesOffsets()                  const;
    GLuint                            uboHandle()                         const;
    GLint                             blockSize()                         const;

private:

    //--------ATTRIBUTES-----------//
    const ShaderProgram&              _shaderProgram;
    const std::string                 _blockName;
    const GLint                       _blockIndex;
    const GLint                       _blockSize;
    const GLuint                      _uboHandle;
    const std::vector<const char*>    _variablesNames;
    const std::vector<GLuint>         _variablesIndices;
    const std::vector<GLint>          _variablesOffsets;

    //--------CONST METHODS--------//

    virtual const std::vector<GLbyte>& dataBuffer()                   const = 0;
    virtual std::vector<GLbyte>       createDataBuffer()              const = 0;
    virtual void                      fillDataBuffer(
                                                std::vector<GLbyte>& dataBuffer)
                                                                      const = 0;

    GLint                             createBlockIndex()                  const;
    GLint                             createBlockSize()                   const;
    GLuint                            createUboHandle()                   const;
    std::vector<GLuint>               createVariablesIndices()            const;
    std::vector<GLint>                createVariablesOffsets()            const;

    const std::vector<const char*>&   variablesNames()                    const;
    const std::vector<GLuint>&        variablesIndices()                  const;


    //----------METHODS------------//
    void                              deleteVariablesNamesInfo();
    void                              configureDataBuffer(
                                                const ShaderProgram& sp,
                                                const std::string& name);

    //--------STATIC METHODS-------//
    static std::vector<const char*>   getStringsStoredLinearly(
                                                const std::vector<std::string>&
                                                  strNames);
    static std::vector<const char*>   copyVariablesNamesInfo(
                                                const std::vector<const char*>&
                                                  varNamesInfo);


};

#endif /* UNIFORMBLOCK_H */

