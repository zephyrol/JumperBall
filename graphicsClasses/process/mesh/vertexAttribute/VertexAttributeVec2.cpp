//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeVec2.h"

VertexAttributeVec2::VertexAttributeVec2(std::vector<glm::vec2> &&data)
    : VertexAttribute(std::move(data), GL_FLOAT) {
}

