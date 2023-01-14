//
// Created by S.Morgenthaler on 08/01/2023.
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

    ~VertexAttributeFloat() override = default;

    std::function<void(GLuint)> getVertexAttribPointerFunc() const override;
};


#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEFLOAT_H
