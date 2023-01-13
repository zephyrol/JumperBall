//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeFloat.h"

VertexAttributeFloat::VertexAttributeFloat(std::vector<GLfloat> &&data)
    : VertexAttribute(std::move(data)) {
}

std::function<void(GLuint)> VertexAttributeFloat::getVertexAttribPointerFunc() const {
    return [](GLuint index) {
        return glVertexAttribPointer(
            index, // Vertex attribute index
            1, // Number of components (only one float)
            GL_FLOAT, // Data type
            GL_FALSE,
            0,
            nullptr
        );
    };
}
