//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributeFloat.h"

VertexAttributeFloat::VertexAttributeFloat(std::vector<GLfloat> &&data)
    : VertexAttribute(std::move(data), GL_FLOAT) {
}
