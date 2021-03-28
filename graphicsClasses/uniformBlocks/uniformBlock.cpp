/*
 * File: uniformBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 décembre 2019, 12:22
 */

#include "uniformBlock.h"
#include <cstring>

UniformBlock::UniformBlock(const std::string& blockName,
                           const std::vector <std::string>& variablesNames):
    _uboHandle(createUboHandle()),
    _blockName(blockName),
    _variablesNames(getStringsStoredLinearly(variablesNames)),
    _blockSizes{},
    _variablesOffsets{},
    _bufferData{} {

}

std::vector <const char*> UniformBlock::getStringsStoredLinearly (const std::vector <std::string>& strNames) {

    std::vector <const char*> names(strNames.size());

    for (size_t i = 0; i < strNames.size(); ++i) {

        const std::string& strName = strNames.at(i);
        const char*cName = strName.c_str();
        char*cNameAllocated = new char[strName.length() + 1];


#if defined(__STDC_LIB_EXT1__) || defined(_MSC_VER)
        strncpy_s(cNameAllocated, strName.length() + 1, cName, strName.length() + 1);
#else
        strncpy(cNameAllocated, cName, strName.length() + 1);
#endif
        names[i] = cNameAllocated;
    }

    return names;
}


GLuint UniformBlock::createBlockIndex (const CstShaderProgram_uptr& sp) const {
    return glGetUniformBlockIndex(sp->getHandle(), _blockName.c_str());
}

void UniformBlock::createBlockSize (const CstShaderProgram_uptr& sp) {
    const GLuint blockIndex = createBlockIndex(sp);
    // Getting the size of the uniform block for the shader program
    GLint blockSize;
    glGetActiveUniformBlockiv(sp->getHandle(), blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    _blockSizes[sp->getHandle()] = blockSize;
}


GLuint UniformBlock::createUboHandle() const {
    GLuint uboHandle;
    glGenBuffers(1, &uboHandle);
    return uboHandle;
}

std::vector <GLuint> UniformBlock::createVariablesIndices (const CstShaderProgram_uptr& sp) const {
    std::vector <GLuint> variablesIndices(_variablesNames.size());

    // Getting the indices for each variable
    glGetUniformIndices(sp->getHandle(),
                        static_cast <GLsizei>(_variablesNames.size()),
                        _variablesNames.data(),
                        variablesIndices.data());
    return variablesIndices;
}

void UniformBlock::createVariablesOffsets (const CstShaderProgram_uptr& sp) {
    const std::vector <GLuint> variablesIndices = createVariablesIndices(sp);
    std::vector <GLint> variablesOffsets(_variablesNames.size());

    // Getting the offset in the buffer for each variable
    glGetActiveUniformsiv(sp->getHandle(),
                          static_cast <GLsizei>(variablesIndices.size()),
                          variablesIndices.data(), GL_UNIFORM_OFFSET,
                          variablesOffsets.data());

    _variablesOffsets[sp->getHandle()] = variablesOffsets;
}

const std::vector <const char*>& UniformBlock::variablesNames() const {
    return _variablesNames;
}

void UniformBlock::deleteVariablesNamesInfo() {
    for (size_t i = 0; i < _variablesNames.size(); ++i) {
        delete[]_variablesNames[i];
    }
}

UniformBlock::~UniformBlock() {
    deleteVariablesNamesInfo();
}

const std::string& UniformBlock::name() const {
    return _blockName;
}

void UniformBlock::bind (const CstShaderProgram_uptr& sp) const {
    glBindBuffer(GL_UNIFORM_BUFFER, _uboHandle);
    const GLuint handle = sp->getHandle();
    glBufferData(GL_UNIFORM_BUFFER, _blockSizes.at(handle), _bufferData.at(handle).data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, _uboHandle);
}

void UniformBlock::registerShader (const CstShaderProgram_uptr& sp) {
    const GLuint spHandle = sp->getHandle();
    createBlockSize(sp);
    createVariablesOffsets(sp);
    _bufferData[spHandle] = std::vector <GLbyte>(_blockSizes.at(spHandle));
    update();
}
