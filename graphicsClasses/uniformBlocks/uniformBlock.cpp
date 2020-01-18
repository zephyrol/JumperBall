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
  _variablesNames(getStringsStoredLinearly(variablesNames)),
  _uboHandle()
{
  glGenBuffers(1, &_uboHandle);
}

std::pair<GLint,std::vector<GLint> >UniformBlock::configureDataBuffer(
                                                const ShaderProgram& sp, 
                                                const std::string& name) const{

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
