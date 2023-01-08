//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#include "VertexAttributes.h"

VertexAttributes::VertexAttributes(
    vecVertexAttributeVec3_uptr attributesVec3,
    vecVertexAttributeVec2_uptr attributesVec2,
    vecVertexAttributeFloat_uptr attributesFloat,
    vecVertexAttributeInt_uptr attributesInt
) :
    _attributesVec3(std::move(attributesVec3)),
    _attributesVec2(std::move(attributesVec2)),
    _attributesFloat(std::move(attributesFloat)),
    _attributesInt(std::move(attributesInt)) {
}

size_t VertexAttributes::getNumberOfVertices() const {
    // The first vec 3 vertex attribute on is always the position that corresponds to the number of vertices.
    return _attributesVec3.front()->dataLength();
}

void VertexAttributes::merge(VertexAttributes &&other) {
    mergeVertexAttributes(_attributesVec3, moveAttributesVec3());
    mergeVertexAttributes(_attributesVec2, moveAttributesVec2());
    mergeVertexAttributes(_attributesFloat, moveAttributesFloat());
    mergeVertexAttributes(_attributesInt, moveAttributesInt());
}

vecVertexAttributeVec3_uptr &&VertexAttributes::moveAttributesVec3() {
    return std::move(_attributesVec3);
}

vecVertexAttributeVec2_uptr &&VertexAttributes::moveAttributesVec2() {
    return std::move(_attributesVec2);
}

vecVertexAttributeFloat_uptr &&VertexAttributes::moveAttributesFloat() {
    return std::move(_attributesFloat);
}

vecVertexAttributeInt_uptr &&VertexAttributes::moveAttributesInt() {
    return std::move(_attributesInt);
}

template<typename T>
void VertexAttributes::mergeVertexAttributes(T &own, T &&other) {
    for (size_t i = 0; i < own.size(); ++i) {
        own[i]->merge(std::move(other[i]));
    }
}

VertexAttributeVec3_uptr VertexAttributes::genVertexAttribute(std::vector<glm::vec3> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeVec3>(std::move(vertexAttributeData));
}

VertexAttributeVec2_uptr VertexAttributes::genVertexAttribute(std::vector<glm::vec2> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeVec2>(std::move(vertexAttributeData));
}

VertexAttributeFloat_uptr VertexAttributes::genVertexAttribute(std::vector<GLfloat> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeFloat>(std::move(vertexAttributeData));
}

VertexAttributeInt_uptr VertexAttributes::genVertexAttribute(std::vector<GLint> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeInt>(std::move(vertexAttributeData));
}

template<typename VertexAttributeType, typename VertexAttributeData>
std::unique_ptr<VertexAttributeType> VertexAttributes::genVertexAttributeCore(
    VertexAttributeData &&vertexAttributeData
) {
    // Return null if the vertex attribute does not need to be created.
    if (vertexAttributeData.empty()) {
        return nullptr;
    }
    return std::unique_ptr<VertexAttributeType>(
        new VertexAttributeType(std::forward<VertexAttributeData>(vertexAttributeData))
    );
}


