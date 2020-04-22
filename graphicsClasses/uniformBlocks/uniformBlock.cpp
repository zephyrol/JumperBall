/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uniformBlock.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 22 décembre 2019, 12:22
 */

#define __STDC_WANT_LIB_EXT1__ 1
#include "uniformBlock.h"
#include <cstring>

UniformBlock::UniformBlock(const std::vector<std::string>& variablesNames) :
  _dataBuffer(),
  _variablesNames(getStringsStoredLinearly(variablesNames)),
  _variablesIndices(variablesNames.size()),
  _variablesOffsets(variablesNames.size()),
  _blockSize(0),
  _uboHandle()
{
  glGenBuffers(1, &_uboHandle);
}


UniformBlock::UniformBlock(const UniformBlock& uniformBlock) :
  _dataBuffer(uniformBlock._dataBuffer),
  _variablesNames(copyVariablesNamesInfo(uniformBlock._variablesNames)),
  _variablesIndices(uniformBlock._variablesIndices),
  _variablesOffsets(uniformBlock._variablesOffsets),
  _blockSize(uniformBlock.blockSize()),
  _uboHandle()
{
  glGenBuffers(1, &_uboHandle); //The uboHandle is always unique
}

UniformBlock& UniformBlock::operator=(const UniformBlock& uniformBlock) {

   //We do not change the uboHandle ... it is unique
  _dataBuffer                       = uniformBlock.dataBuffer();
  _variablesIndices                 = uniformBlock.variablesIndices();
  _variablesOffsets                 = uniformBlock.variablesOffsets();
  _blockSize                        = uniformBlock.blockSize();

  deleteVariablesNamesInfo();
  std::vector<const char*>& newInfo = 
                const_cast<std::vector<const char*>&> (_variablesNames);
  newInfo                           = copyVariablesNamesInfo(
                                        uniformBlock.variablesNames());

  return *this;
}


void UniformBlock::configureDataBuffer( const ShaderProgram& sp, 
                                        const std::string& name){

    const GLuint blockIndex =
                          glGetUniformBlockIndex(sp.getHandle(),name.c_str());

    //Getting the size of the uniform block for the shader program
    glGetActiveUniformBlockiv ( sp.getHandle(),blockIndex,
                                GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize);

    //Getting the indices for each variable
    glGetUniformIndices       ( sp.getHandle(),
                                static_cast<GLsizei>(_variablesNames.size()),
                                _variablesNames.data(),
                                _variablesIndices.data());
    //Getting the offset in the buffer for each variable
    glGetActiveUniformsiv     ( sp.getHandle(),
                                static_cast<GLsizei>(_variablesIndices.size()), 
                                _variablesIndices.data(), GL_UNIFORM_OFFSET,
                                _variablesOffsets.data());

    if (_blockSize >= 0 ) {
        unsigned int uSize = static_cast<unsigned int> (_blockSize);
        if (_dataBuffer.size() != uSize) {
            _dataBuffer.resize(uSize);
        }
    }

}



std::vector<const char*> UniformBlock::getStringsStoredLinearly(
                                                const std::vector<std::string>&
                                                strNames) {

    std::vector<const char*> names (strNames.size());

    for ( size_t i = 0; i < strNames.size(); ++i)  {

        const std::string&  strName         = strNames.at(i);
        const char*         cName           = strName.c_str();
        char*               cNameAllocated  = new char[strName.length()+1];


        #if defined (__STDC_LIB_EXT1__) || defined (_MSC_VER)
        strncpy_s(cNameAllocated,strName.length()+1,cName,strName.length()+1);
        #else
        strncpy(cNameAllocated,cName,strName.length()+1);
        #endif
        names[i] = cNameAllocated;
    }

    return names;
}

std::vector<const char*> UniformBlock::copyVariablesNamesInfo(
                        const std::vector<const char*>& varNamesInfo) {

    std::vector<const char*> names (varNamesInfo.size());

    for ( size_t i = 0; i < varNamesInfo.size(); ++i)  {

        size_t              length = strlen(varNamesInfo[i]);
        char*               cNameAllocated  = new char[length+1];

        #if defined (__STDC_LIB_EXT1__) || defined (_MSC_VER)
        strncpy_s(cNameAllocated,length+1,varNamesInfo[i],length+1);
        #else
        strncpy(cNameAllocated,varNamesInfo[i],length+1);
        #endif

        names[i] = cNameAllocated;
    }
    

    return names;
}


const std::vector<GLuint>& UniformBlock::variablesIndices() const {
    return _variablesIndices;
}

const std::vector<const char*>& UniformBlock::variablesNames() const {
    return _variablesNames;
}

const std::vector<GLint>& UniformBlock::variablesOffsets() const {
    return _variablesOffsets;

}

const std::vector<GLchar>& UniformBlock::dataBuffer() const{
    return _dataBuffer;
}


GLint UniformBlock::blockSize() const {
    return _blockSize;
}

GLuint UniformBlock::uboHandle() const {
    return _uboHandle;
}

void UniformBlock::deleteVariablesNamesInfo(){
   
    for (size_t i = 0; i < _variablesNames.size(); ++i) {
        delete[] _variablesNames[i];
    }
}

UniformBlock::~UniformBlock() {
    deleteVariablesNamesInfo();
}

