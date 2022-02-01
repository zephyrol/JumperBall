//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
#define JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>


class UniformBuffer {

public:

    UniformBuffer(
        const std::string& name,
        const vecCstShaderProgram_sptr& shaderPrograms,
        size_t bufferSize
    );

    void bindBuffer() const;
    template<class T> void fillBufferData(size_t offset, const T& value);
    void fillBufferData(const std::vector<GLubyte>& values);
    static void unbindBuffer();

    void freeGPUMemory();

protected:
    const std::string _name;
    const GLsizeiptr _bufferSize;
    const vecCstShaderProgram_sptr _shaderPrograms;
    const GLuint _ubo;
    GLuint createUbo() const;
};

template<class T>
void UniformBuffer::fillBufferData(size_t offset, const T &value) {
    /*glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, _bufferSize, &value[0], GL_DYNAMIC_DRAW);*/
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &value[0]);
}

#endif //JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
