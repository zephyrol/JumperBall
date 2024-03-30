//
// Created by S.Morgenthaler on 30/03/2024.
//

#include "GpuVertexArray.h"

GpuVertexArray::GpuVertexArray(): _vertexArrayId(
[]() {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        return vao;
    }()
) {
}

void GpuVertexArray::bind() const {
    glBindVertexArray(_vertexArrayId);
}

GpuVertexArray::~GpuVertexArray() {
    glDeleteVertexArrays(1, &_vertexArrayId);
}
