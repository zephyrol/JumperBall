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
#include "process/Mesh.h"
#include <cstring>

class UniformBlock;
using UniformBlock_sptr = std::shared_ptr <UniformBlock>;
using CstUniformBlock_sptr = std::shared_ptr <const UniformBlock>;
class UniformBlock {
public:

UniformBlock(Mesh::UniformVariables <glm::vec3>&& variablesVecThree);

UniformBlock(const UniformBlock& uniformBlock) = delete;
UniformBlock& operator= (const UniformBlock& uniformBlock) = delete;

void update(const std::string& varName, const glm::vec3& value);
void bind(const CstShaderProgram_sptr& sp) const;
void registerShader(const CstShaderProgram_sptr& sp, const std::string& blockName);

void freeGPUMemory();

struct ShaderBlock {
    std::vector <GLint> variablesOffsets;
    std::vector <GLbyte> buffer;
};

private:

const GLuint _uboHandle;

Mesh::UniformVariables <glm::vec3> _variablesVecThree;
const std::vector <const char*> _linearVariablesNames;

std::unordered_map <CstShaderProgram_sptr, ShaderBlock> _shaderBlocks;

GLuint createUboHandle() const;

GLint getBlockSize(const CstShaderProgram_sptr& sp, const std::string& blockName) const;
std::vector <GLuint> createVariablesIndices(const CstShaderProgram_sptr& sp) const;
std::vector <GLint> createVariablesOffsets(const CstShaderProgram_sptr& sp) const;
ShaderBlock createShaderBlock(const CstShaderProgram_sptr& sp, const std::string& blockName) const;

void deleteVariablesNamesInfo();
std::vector <const char*> getStringsStoredLinearly() const;

template<typename T> void fillBlockBuffer(
    const std::vector <GLint>& variableOffsets,
    const std::vector <T>& values,
    std::vector <GLbyte>& blockBuffer
    ) const;

};

template<typename T> void UniformBlock::fillBlockBuffer (
    const std::vector <GLint>& variableOffsets,
    const std::vector <T>& values,
    std::vector <GLbyte>& blockBuffer
    ) const {
    for (size_t i = 0; i < variableOffsets.size(); ++i) {
        memcpy(blockBuffer.data() + variableOffsets.at(i), &values.at(i), sizeof(T));
    }
}


#endif /* UNIFORMBLOCK_H */
