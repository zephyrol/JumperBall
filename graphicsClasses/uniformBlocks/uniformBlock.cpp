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

UniformBlock::UniformBlock( const ShaderProgram& sp, const std::string name,
                            const std::vector<std::string>& variablesNames) :
  _blockIndex(glGetUniformBlockIndex(sp.getHandle(),name.c_str())),
  _blockSize(),
  _dataInsideBlock(),
  _variablesNames(variablesNames),
  _variablesIndices(variablesNames.size()),
  _variablesOffset(variablesNames.size()),
  _uboHandle()
{
  const GLchar* *const cStrVarNames = new const GLchar* [variablesNames.size()];

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

  delete[] cStrVarNames;

  glGenBuffers(1, &_uboHandle);
}

void UniformBlock::bind() const {
    glBindBufferBase(GL_UNIFORM_BUFFER,0, _uboHandle);
}



UniformBlock::~UniformBlock() {
}

