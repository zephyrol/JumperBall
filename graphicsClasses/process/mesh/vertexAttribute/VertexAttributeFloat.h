//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTEFLOAT_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTEFLOAT_H
#include "VertexAttribute.h"

class VertexAttributeFloat;
using VertexAttributeFloat_uptr = std::unique_ptr<VertexAttributeFloat>;
using vecVertexAttributeFloat_uptr = std::vector<VertexAttributeFloat_uptr>;

class VertexAttributeFloat: public VertexAttribute<GLfloat> {
public:
    explicit VertexAttributeFloat(std::vector<GLfloat> &&data);

    GLint numberOfComponents() const override;

    ~VertexAttributeFloat() override = default;
};


#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEFLOAT_H
