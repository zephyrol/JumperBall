//
// Created by S.Morgenthaler on 08/01/2023.
//

#include "VertexAttributeVec2.h"

VertexAttributeVec2::VertexAttributeVec2(std::vector<glm::vec2> &&data)
    : VertexAttribute(std::move(data)) {
}

std::function<void(GLuint)> VertexAttributeVec2::getVertexAttribPointerFunc() const {
    return [](GLuint index) {
        return glVertexAttribPointer(
            index, // Vertex attribute index
            2, // Number of components (two floats)
            GL_FLOAT, // Data type
            GL_FALSE,
            0,
            nullptr
        );
    };
}
