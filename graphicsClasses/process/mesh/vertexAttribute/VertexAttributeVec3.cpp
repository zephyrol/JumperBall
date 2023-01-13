//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeVec3.h"

VertexAttributeVec3::VertexAttributeVec3(std::vector<glm::vec3> &&data)
    : VertexAttribute(std::move(data)) {
}

std::function<void(GLuint)> VertexAttributeVec3::getVertexAttribPointerFunc() const {
    return [](GLuint index) {
        return glVertexAttribPointer(
            index, // Vertex attribute index
            3, // Number of components (three floats)
            GL_FLOAT, // Data type
            GL_FALSE,
            0,
            nullptr
        );
    };
}

