//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
#define JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
#include "ShaderProgram.h"


class UniformBuffer {

public:

    using ShaderProgramBindingPoint = std::unordered_map<CstShaderProgram_sptr, GLuint>;

    UniformBuffer(
        const std::string& name,
        ShaderProgramBindingPoint&& shaderProgramsBindingPoint,
        size_t bufferSize
    );

    template<class T> void fillBufferData(size_t offset, const T& value);
    void freeGPUMemory();

private:
    const std::string _name;
    const GLsizeiptr _bufferSize;
    const ShaderProgramBindingPoint _shaderProgramToBindingPoint;
    const GLuint _ubo;

    GLuint createUbo() const;
};

template<class T>
void UniformBuffer::fillBufferData(size_t offset, const T &value) {
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &value);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


#endif //JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
