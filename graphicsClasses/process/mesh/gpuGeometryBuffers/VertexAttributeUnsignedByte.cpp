//
// Created by S.Morgenthaler on 14/01/2023.
//

#include "VertexAttributeUnsignedByte.h"

VertexAttributeUnsignedByte::VertexAttributeUnsignedByte(std::vector<GLubyte> &&data)
    : VertexAttribute(std::move(data)) {
}

std::function<void(GLuint)> VertexAttributeUnsignedByte::getVertexAttribPointerFunc() const {
    return [](GLuint index) {
        return glVertexAttribIPointer(
            index, // Vertex attribute index
            1, // Number of components (only one unsigned byte)
            GL_UNSIGNED_BYTE, // Data type
            0,
            nullptr
        );
    };
}
