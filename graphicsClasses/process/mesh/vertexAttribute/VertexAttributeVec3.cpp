//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeVec3.h"

VertexAttributeVec3::VertexAttributeVec3(std::vector<glm::vec3> &&data)
    : VertexAttribute(std::move(data), GL_FLOAT) {
}

GLint VertexAttributeVec3::numberOfComponents() const {
    return 3;
}

