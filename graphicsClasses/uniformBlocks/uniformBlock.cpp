/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uniformBlock.cpp
 * Author: morgenthaler
 * 
 * Created on 22 décembre 2019, 12:22
 */

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

void UniformBlock::configureDataBuffer( const ShaderProgram& sp, 
                                        const std::string& name){

    const GLuint blockIndex =
                          glGetUniformBlockIndex(sp.getHandle(),name.c_str());

    //Getting the size of the uniform block for the shader program
    glGetActiveUniformBlockiv ( sp.getHandle(),blockIndex,
                                GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize);

    //Getting the indices for each variable
    glGetUniformIndices       ( sp.getHandle(), _variablesNames.number,
                                _variablesNames.names,_variablesIndices.data());
    //Getting the offset in the buffer for each variable
    glGetActiveUniformsiv     ( sp.getHandle(),  _variablesIndices.size(), 
                                _variablesIndices.data(), GL_UNIFORM_OFFSET,
                                _variablesOffsets.data());

    if (_blockSize >= 0 ) {
        unsigned int uSize = static_cast<unsigned int> (_blockSize);
        if (_dataBuffer.size() != uSize) {
            _dataBuffer.resize(uSize);
        }
    }

}



UniformBlock::variablesNamesInfo UniformBlock::getStringsStoredLinearly(
                                                const std::vector<std::string>&
                                                strNames) {

    char** names = new char* [strNames.size()];

    for ( size_t i = 0; i < strNames.size(); ++i)  {

        const std::string&  strName         = strNames.at(i);
        const char*         cName           = strName.c_str();
        char*               cNameAllocated  = new char[strName.length()+1];

        strncpy(cNameAllocated,cName,strName.length()+1);
        names[i] = cNameAllocated;
    }

    UniformBlock::variablesNamesInfo infoNames {names,strNames.size()};
    return infoNames ;
}

const std::vector<GLuint>& UniformBlock::variablesIndices() const {
    return _variablesIndices;
}

const UniformBlock::variablesNamesInfo& UniformBlock::variablesNames() const {
    return _variablesNames;
}

const std::vector<GLint>& UniformBlock::variablesOffsets() const {
    return _variablesOffsets;

}

GLint UniformBlock::blockSize() const {
    return _blockSize;
}

GLuint UniformBlock::uboHandle() const {
    return _uboHandle;
}


UniformBlock::~UniformBlock() {

    for (size_t i = 0; i < _variablesNames.number; ++i) {
        delete[] _variablesNames.names[i];
    }
    delete[] _variablesNames.names;
}

