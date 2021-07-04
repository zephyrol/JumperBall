/*
 * File: UniformBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 décembre 2019, 12:22
 */

#include "UniformBlock.h"

UniformBlock::UniformBlock(Mesh::UniformVariables <glm::vec3>&& variablesVecThree):
    _uboHandle(createUboHandle()),
    _variablesVecThree(std::move(variablesVecThree)),
    _linearVariablesNames(getStringsStoredLinearly()),
    _shaderBlocks{} {
}

void UniformBlock::update (const std::string& varName, const glm::vec3& value) {
    _variablesVecThree.at(varName) = value;

    const auto& _variablesVecThreeValues = Mesh::extractUniformVariablesValues(_variablesVecThree);
    for (auto& shaderNameAndBlock : _shaderBlocks) {
        ShaderBlock& shaderBlock = shaderNameAndBlock.second;
        fillBlockBuffer(shaderBlock.variablesOffsets, _variablesVecThreeValues, shaderBlock.buffer);
    }
}

std::vector <const char*> UniformBlock::getStringsStoredLinearly() const {
    std::vector <const char*> linearVariablesNames;
    for (const auto& variableVecThree : _variablesVecThree) {
        const std::string& variableName = variableVecThree.first;
        linearVariablesNames.push_back(variableName.c_str());

    }
    return linearVariablesNames;
}

GLint UniformBlock::getBlockSize (const CstShaderProgram_sptr& sp, const std::string& blockName) const {
    const GLuint blockIndex = glGetUniformBlockIndex(sp->getHandle(), blockName.c_str());
    // Getting the size of the uniform block for the shader program
    GLint blockSize;
    glGetActiveUniformBlockiv(sp->getHandle(), blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    return blockSize;
}


GLuint UniformBlock::createUboHandle() const {
    GLuint uboHandle;
    glGenBuffers(1, &uboHandle);
    return uboHandle;
}

std::vector <GLuint> UniformBlock::createVariablesIndices (const CstShaderProgram_sptr& sp) const {
    const size_t numberOfVariables = _linearVariablesNames.size();
    std::vector <GLuint> variablesIndices(numberOfVariables);

    // Getting the indices for each variable
    glGetUniformIndices(sp->getHandle(),
                        static_cast <GLsizei>(numberOfVariables),
                        _linearVariablesNames.data(),
                        variablesIndices.data());
    return variablesIndices;
}

std::vector <GLint> UniformBlock::createVariablesOffsets (const CstShaderProgram_sptr& sp) const {
    const std::vector <GLuint> variablesIndices = createVariablesIndices(sp);
    std::vector <GLint> variablesOffsets(variablesIndices.size());

    // Getting the offset in the buffer for each variable
    glGetActiveUniformsiv(sp->getHandle(),
                          static_cast <GLsizei>(variablesIndices.size()),
                          variablesIndices.data(),
                          GL_UNIFORM_OFFSET,
                          variablesOffsets.data());

    return variablesOffsets;
}

UniformBlock::ShaderBlock UniformBlock::createShaderBlock (
    const CstShaderProgram_sptr& sp,
    const std::string& blockName
    ) const {
    const std::vector <GLint> variablesOffsets = createVariablesOffsets(sp);
    const GLint blockSize = getBlockSize(sp, blockName);
    std::vector <GLbyte> blockBuffer(blockSize);
    fillBlockBuffer(variablesOffsets, Mesh::extractUniformVariablesValues(_variablesVecThree), blockBuffer);

    return { variablesOffsets, blockBuffer };
}


void UniformBlock::bind (const CstShaderProgram_sptr& sp) const {
    glBindBuffer(GL_UNIFORM_BUFFER, _uboHandle);
    const UniformBlock::ShaderBlock& shaderBlock = _shaderBlocks.at(sp);
    const std::vector <GLbyte>& shaderBlockBuffer = shaderBlock.buffer;
    glBufferData(GL_UNIFORM_BUFFER, shaderBlockBuffer.size(), shaderBlockBuffer.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, _uboHandle);
}


void UniformBlock::registerShader (const CstShaderProgram_sptr& sp, const std::string& blockName) {
    const UniformBlock::ShaderBlock shaderBlock = createShaderBlock(sp, blockName);
    _shaderBlocks[sp] = shaderBlock;
}

void UniformBlock::freeGPUMemory() {
    glDeleteBuffers(1, &_uboHandle);
}
