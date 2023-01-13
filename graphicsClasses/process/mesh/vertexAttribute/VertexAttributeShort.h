//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTESHORT_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTESHORT_H
#include "VertexAttribute.h"

class VertexAttributeShort;
using VertexAttributeShort_uptr = std::unique_ptr<VertexAttributeShort>;
using vecVertexAttributeShort_uptr = std::vector<VertexAttributeShort_uptr>;

class VertexAttributeShort: public VertexAttribute<GLshort> {
public:
    explicit VertexAttributeShort(std::vector<GLshort> &&data);

    ~VertexAttributeShort() override = default;

    std::function<void(GLuint)> getVertexAttribPointerFunc() const override;
};


#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTESHORT_H
