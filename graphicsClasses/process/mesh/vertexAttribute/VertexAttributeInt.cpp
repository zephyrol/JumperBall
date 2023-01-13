//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeInt.h"

VertexAttributeInt::VertexAttributeInt(std::vector<GLshort> &&data)
    : VertexAttribute(std::move(data), GL_SHORT) {
}

GLint VertexAttributeInt::numberOfComponents() const {
    return 1;
}
