//
// Created by S.Morgenthaler on 14/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTEUNSIGNEDBYTE_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTEUNSIGNEDBYTE_H
#include "VertexAttribute.h"

class VertexAttributeUnsignedByte;
using VertexAttributeUnsignedByte_uptr = std::unique_ptr<VertexAttributeUnsignedByte>;
using vecVertexAttributeUnsignedByte_uptr = std::vector<VertexAttributeUnsignedByte_uptr>;

class VertexAttributeUnsignedByte : public VertexAttribute<GLubyte> {
public:
    explicit VertexAttributeUnsignedByte(std::vector<GLubyte> &&data);

    ~VertexAttributeUnsignedByte() override = default;

    std::function<void(GLuint)> getVertexAttribPointerFunc() const override;
};


#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEUNSIGNEDBYTE_H
