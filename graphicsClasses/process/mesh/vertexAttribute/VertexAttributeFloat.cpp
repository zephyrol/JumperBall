//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeFloat.h"

VertexAttributeFloat::VertexAttributeFloat(std::vector<GLfloat> &&data)
    : VertexAttribute(std::move(data), GL_FLOAT) {
}

std::function<void(GLuint)> VertexAttributeFloat::getVertexAttribPointerFunc() const {
    return [](GLuint index) {
        return glVertexAttribPointer(
            _index, // Index
            1, // Number of components
            GL_FLOAT, // Data type
            0,
            nullptr
        );
    };
}
