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

UniformBlock::UniformBlock( //const ShaderProgram& sp,
                            const std::vector<std::string>& variablesNames) :
  //_blockIndex(glGetUniformBlockIndex(sp.getHandle(),name.c_str())),
  //_blockSize(),
  //_dataInsideBlock(),
  _variablesNames(getStringsStoredLinearly(variablesNames)),
  //_variablesNames(variablesNames),
  /*_variablesIndices(variablesNames.size()),
  _variablesOffset(variablesNames.size()),*/
  _uboHandle()
{
  /*const GLchar* *const cStrVarNames = new const GLchar* [variablesNames.size()];

  for (size_t i = 0 ; i < variablesNames.size(); ++i) {
      cStrVarNames[i]     = variablesNames.at(i).c_str();
  }
  
  glGetActiveUniformBlockiv ( sp.getHandle(),_blockIndex,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize);
  glGetUniformIndices       ( sp.getHandle(),_variablesIndices.size(),
                              cStrVarNames,_variablesIndices.data());
  glGetActiveUniformsiv     ( sp.getHandle(),_variablesOffset.size(), 
                              _variablesIndices.data(), GL_UNIFORM_OFFSET,
                              _variablesOffset.data());

  _dataInsideBlock.resize(_blockSize);

  delete[] cStrVarNames;*/

  glGenBuffers(1, &_uboHandle);
}

std::pair<GLint,std::vector<GLint> >UniformBlock::configureDataBuffer(
                                                const ShaderProgram& sp, 
                                                const std::string& name) const{

    /*constexpr size_t maxNumberOfVariables = 32;
    const GLchar* cStrVarNames[maxNumberOfVariables];

    std::vector<const char*> tab (_variablesNames.size());
    for (size_t i = 0 ; i < _variablesNames.size(); ++i) {
        tab[i]     = _variablesNames.at(i).c_str();
    }

    for (size_t i = 0 ; i < _variablesNames.size(); ++i) {
        cStrVarNames[i]     = _variablesNames.at(i).c_str();
    }*/

    const GLuint blockIndex =
                          glGetUniformBlockIndex(sp.getHandle(),name.c_str());
    GLint blockSize;
    std::vector<GLuint> variablesIndices  (_variablesNames.number);
    std::vector<GLint>  variablesOffset   (_variablesNames.number);

    //Getting the size of the uniform block for the shader program
    glGetActiveUniformBlockiv ( sp.getHandle(),blockIndex,
                                GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    //Getting the indices for each variable
    glGetUniformIndices       ( sp.getHandle(), _variablesNames.number,
                                _variablesNames.names,variablesIndices.data());

    //Getting the offset in the buffer for each variable
    glGetActiveUniformsiv     ( sp.getHandle(),  variablesIndices.size(), 
                                variablesIndices.data(), GL_UNIFORM_OFFSET,
                                variablesOffset.data());

    std::pair<GLint,std::vector<GLint> > infoDataBuffer;
    infoDataBuffer.first  = blockSize;
    infoDataBuffer.second = variablesOffset;

    return infoDataBuffer;
    //_dataInsideBlock.resize(blockSize);
    //glBindBufferBase(GL_UNIFORM_BUFFER, 0, _uboHandle);
}



UniformBlock::variablesNames UniformBlock::getStringsStoredLinearly(
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

    UniformBlock::variablesNames infoNames {names,strNames.size()};
    return infoNames ;
}




UniformBlock::~UniformBlock() {

    for (size_t i = 0; i < _variablesNames.number; ++i) {
        delete[] _variablesNames.names[i];
    }
    delete[] _variablesNames.names;
}
