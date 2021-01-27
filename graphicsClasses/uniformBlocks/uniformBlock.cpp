
/* 
 * File:   uniformBlock.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 22 décembre 2019, 12:22
 */

#include "uniformBlock.h"
#include <cstring>

UniformBlock::UniformBlock(const std::string &blockName,
                           const ShaderProgram& shaderProgram,
                           const std::vector<std::string> &variablesNames):
  _shaderProgram(shaderProgram),
  _blockName(blockName),
  _blockIndex(createBlockIndex()),
  _blockSize(createBlockSize()),
  _uboHandle(createUboHandle()),
  _variablesNames(getStringsStoredLinearly(variablesNames)),
  _variablesIndices(createVariablesIndices()),
  _variablesOffsets(createVariablesOffsets())
{
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

GLint UniformBlock::createBlockIndex() const{
   GLuint blockIndex = glGetUniformBlockIndex(_shaderProgram.getHandle(),
                                              _blockName.c_str());
   return blockIndex;
}

GLint UniformBlock::createBlockSize() const{
    //Getting the size of the uniform block for the shader program
    GLint blockSize;
    glGetActiveUniformBlockiv ( _shaderProgram.getHandle(),_blockIndex,
                                GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    return blockSize;
}

GLuint UniformBlock::createUboHandle() const{
  GLuint uboHandle;
  glGenBuffers(1, &uboHandle);
  return uboHandle;
}

std::vector<GLuint> UniformBlock::createVariablesIndices() const {
    std::vector<GLuint> variablesIndices (_variablesNames.size());

    //Getting the indices for each variable
    glGetUniformIndices       ( _shaderProgram.getHandle(),
                                static_cast<GLsizei>(_variablesNames.size()),
                                _variablesNames.data(),
                                variablesIndices.data());
    return variablesIndices;
}

std::vector<GLint> UniformBlock::createVariablesOffsets() const {
    std::vector<GLint> variablesOffsets (_variablesNames.size());

    //Getting the offset in the buffer for each variable
    glGetActiveUniformsiv     ( _shaderProgram.getHandle(),
                                static_cast<GLsizei>(_variablesIndices.size()),
                                _variablesIndices.data(), GL_UNIFORM_OFFSET,
                                variablesOffsets.data());
    return variablesOffsets;
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

