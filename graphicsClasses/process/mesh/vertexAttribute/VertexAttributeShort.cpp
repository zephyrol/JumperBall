//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeShort.h"

VertexAttributeShort::VertexAttributeShort(std::vector<GLshort> &&data)
    : VertexAttribute(std::move(data)) {
}

std::function<void(GLuint)> VertexAttributeShort::getVertexAttribPointerFunc() const {
    return [](GLuint index) {
        return glVertexAttribIPointer(
            index, // Vertex attribute index
            1, // Number of components (only one short)
            GL_SHORT, // Data type
            0,
            nullptr
        );
    };
}
