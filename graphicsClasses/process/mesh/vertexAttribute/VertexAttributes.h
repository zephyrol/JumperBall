//
// Created by Sebastien Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTES_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTES_H

#include "process/mesh/vertexAttribute/VertexAttribute.h"
#include "process/mesh/vertexAttribute/VertexAttributeVec3.h"
#include "process/mesh/vertexAttribute/VertexAttributeInt.h"
#include "process/mesh/vertexAttribute/VertexAttributeFloat.h"
#include "process/mesh/vertexAttribute/VertexAttributeVec2.h"


class VertexAttributes {

public:

    explicit VertexAttributes(
        vecVertexAttributeVec3_uptr attributesVec3,
        vecVertexAttributeVec2_uptr attributesVec2,
        vecVertexAttributeFloat_uptr attributesFloat,
        vecVertexAttributeInt_uptr attributesInt
    );

    void merge(VertexAttributes &&other);

    size_t getNumberOfVertices() const;

    static VertexAttributeVec3_uptr genVertexAttribute(std::vector<glm::vec3> &&vertexAttributeData);

    static VertexAttributeVec2_uptr genVertexAttribute(std::vector<glm::vec2> &&vertexAttributeData);

    static VertexAttributeFloat_uptr genVertexAttribute(std::vector<GLfloat> &&vertexAttributeData);

    static VertexAttributeInt_uptr genVertexAttribute(std::vector<GLint> &&vertexAttributeData);


    /**
     * Extract the vertex attributes. Warning this is a side effect, the vertex attributes won't
     * exist anymore in the Vertex Attributes object.
     */
    vecVertexAttributeBase_uptr extractVertexAttributes();

private:

    template<typename VertexAttributeType, typename VertexAttributeData>
    static std::unique_ptr<VertexAttributeType> genVertexAttributeCore(
        VertexAttributeData &&vertexAttributeData
    );

    /**
     * Merge a list of vertex attributes that have the same content (cf vec3, vec2, float, int) between
     * two VertexAttributes objects.
     * This method has to be called for each type of vertex attributes.
     * @tparam T The type of vertex attributes.
     * @param own The vertex attributes list that is the target of merging.
     * @param other The vertex attributes list that will be move into the own one.
     */
    template<typename T>
    void mergeVertexAttributes(T &own, T &&other);


    /**
     * Gather two list of vertex attributes moving the second one into the first one.
     * @tparam T The specific type of vertex attributes that extends VertexAttributeBase.
     * @param current List of VertexAttributeBase.
     * @param vertexAttributes List of specific vertex attributes.
     */
    template<typename T>
    void gatherVertexAttributes(
        vecVertexAttributeBase_uptr& current,
        std::vector<std::unique_ptr<T>>&& vertexAttributes
    );

    vecVertexAttributeVec3_uptr &&moveAttributesVec3();

    vecVertexAttributeVec2_uptr &&moveAttributesVec2();

    vecVertexAttributeFloat_uptr &&moveAttributesFloat();

    vecVertexAttributeInt_uptr &&moveAttributesInt();

    vecVertexAttributeVec3_uptr _attributesVec3;
    vecVertexAttributeVec2_uptr _attributesVec2;
    vecVertexAttributeFloat_uptr _attributesFloat;
    vecVertexAttributeInt_uptr _attributesInt;
};


#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTES_H
