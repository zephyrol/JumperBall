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
        const std::vector<std::string>& fieldNames
    );

    void bindBufferRange() const;
    template<class T> void fillBufferData(size_t offset, const T& value);
    void illBufferData(const std::vector<GLubyte>& values);

    void freeGPUMemory();

protected:
    const std::string _name;
    const vecCstShaderProgram_sptr _shaderPrograms;
    const GLsizeiptr _bufferSize;
    const std::vector<std::string> _fieldNames;
    const std::unordered_map<std::string, GLint> _fieldOffsets;
    const GLuint _ubo;
    GLuint createUbo() const;
    void fillBufferData(const std::vector<GLubyte> &values);

private:
    GLsizeiptr getBufferSize() const;
    std::unordered_map<std::string, GLint> getFieldOffsets();
};

template<class T>
void UniformBuffer::fillBufferData(size_t offset, const T &value) {
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), &value[0]);
}


#endif //JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
