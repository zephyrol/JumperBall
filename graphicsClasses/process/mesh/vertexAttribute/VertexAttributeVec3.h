//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTEVEC3_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTEVEC3_H
#include "VertexAttribute.h"

class VertexAttributeVec3;
using VertexAttributeVec3_uptr = std::unique_ptr<VertexAttributeVec3>;
using vecVertexAttributeVec3_uptr = std::vector<VertexAttributeVec3_uptr>;

class VertexAttributeVec3: public VertexAttribute<glm::vec3> {
public:
    explicit VertexAttributeVec3(std::vector<glm::vec3> &&data);

    std::function<void(GLuint)> getVertexAttribPointerFunc() const override;

    ~VertexAttributeVec3() override = default;
};

#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTEVEC3_H
