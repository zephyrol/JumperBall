/*
 * File: uniformBlock.h
 * Author: Morgenthaler S
 *
 * Created on 22 décembre 2019, 12:22
 */

#ifndef UNIFORMBLOCK_H
#define UNIFORMBLOCK_H
#include <iostream>
#include <map>
#include <vector>
#include "ShaderProgram.h"

class UniformBlock {
public:

UniformBlock(const std::string& blockName,
             const std::vector <std::string>& variablesNames);

UniformBlock(const UniformBlock& uniformBlock) = delete;
UniformBlock& operator= (const UniformBlock& uniformBlock) = delete;

virtual ~UniformBlock();

const std::string& name() const;
virtual void update() = 0;
void bind(const CstShaderProgram_uptr& sp) const;
void registerShader(const CstShaderProgram_uptr& sp);

protected:

template<typename T> void fillDataBuffer(size_t variableNumber, const T& value);

private:

const GLuint _uboHandle;
const std::string _blockName;
const std::vector <const char*> _variablesNames;
std::map <GLuint, GLint> _blockSizes;
std::map <GLuint, std::vector <GLint> > _variablesOffsets;
std::map <GLuint, std::vector <GLbyte> > _bufferData;

void createVariablesOffsets(const CstShaderProgram_uptr& sp);
void createBlockSize(const CstShaderProgram_uptr& sp);

GLuint createUboHandle() const;

const std::vector <const char*>& variablesNames() const;

GLuint createBlockIndex(const CstShaderProgram_uptr& sp) const;
std::vector <GLuint> createVariablesIndices(const CstShaderProgram_uptr& sp) const;

void deleteVariablesNamesInfo();

static std::vector <const char*> getStringsStoredLinearly(const std::vector <std::string>& strNames);

};

template<typename T> void UniformBlock::fillDataBuffer (size_t variableNumber, const T& value) {
    for (auto& bufferData : _bufferData) {
        memcpy(
            bufferData.second.data() + _variablesOffsets.at(bufferData.first).at(variableNumber),
            &value,
            sizeof(T));
    }
}

#endif /* UNIFORMBLOCK_H */
