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

VertexAttributeVec3_uptr VertexAttributes::genVertexAttribute(std::vector<glm::vec3> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeVec3>(std::move(vertexAttributeData));
}

VertexAttributeVec2_uptr VertexAttributes::genVertexAttribute(std::vector<glm::vec2> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeVec2>(std::move(vertexAttributeData));
}

VertexAttributeFloat_uptr VertexAttributes::genVertexAttribute(std::vector<GLfloat> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeFloat>(std::move(vertexAttributeData));
}

VertexAttributeInt_uptr VertexAttributes::genVertexAttribute(std::vector<GLshort> &&vertexAttributeData) {
    return genVertexAttributeCore<VertexAttributeInt>(std::move(vertexAttributeData));
}

vecVertexAttributeBase_uptr VertexAttributes::extractVertexAttributes() {
    vecVertexAttributeBase_uptr extractedVertexAttributes;
    gatherVertexAttributes(extractedVertexAttributes, moveAttributesVec3());
    gatherVertexAttributes(extractedVertexAttributes, moveAttributesVec2());
    gatherVertexAttributes(extractedVertexAttributes, moveAttributesFloat());
    gatherVertexAttributes(extractedVertexAttributes, moveAttributesInt());
    return extractedVertexAttributes;
}

void VertexAttributes::operator+=(VertexAttributes &&other) {
    concatVertexAttributes(_attributesVec3, other.moveAttributesVec3());
    concatVertexAttributes(_attributesVec2, other.moveAttributesVec2());
    concatVertexAttributes(_attributesFloat, other.moveAttributesFloat());
    concatVertexAttributes(_attributesInt, other.moveAttributesInt());
}

void VertexAttributes::operator*=(VertexAttributes &&other) {
    mergeVertexAttributes(_attributesVec3, other.moveAttributesVec3());
    mergeVertexAttributes(_attributesVec2, other.moveAttributesVec2());
    mergeVertexAttributes(_attributesFloat, other.moveAttributesFloat());
    mergeVertexAttributes(_attributesInt, other.moveAttributesInt());
}
