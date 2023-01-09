//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeInt.h"

VertexAttributeInt::VertexAttributeInt(std::vector<GLint> &&data)
    : VertexAttribute(std::move(data), GL_INT) {
}

GLint VertexAttributeInt::numberOfComponents() const {
    return 1;
}
