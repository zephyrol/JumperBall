//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTEINT_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTEINT_H
#include "VertexAttribute.h"

class VertexAttributeInt;
using VertexAttributeInt_uptr = std::unique_ptr<VertexAttributeInt>;
using vecVertexAttributeInt_uptr = std::vector<VertexAttributeInt_uptr>;

class VertexAttributeInt: public VertexAttribute<GLint> {
public:
    explicit VertexAttributeInt(std::vector<GLint> &&data);
};


#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEINT_H
