//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTEVEC2_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTEVEC2_H
#include "VertexAttribute.h"

class VertexAttributeVec2;
using VertexAttributeVec2_uptr = std::unique_ptr<VertexAttributeVec2>;
using vecVertexAttributeVec2_uptr = std::vector<VertexAttributeVec2_uptr>;

class VertexAttributeVec2: public VertexAttribute<glm::vec2> {
public:
    explicit VertexAttributeVec2(std::vector<glm::vec2> &&data);

    GLint numberOfComponents() const override;
};

#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEVEC2_H
