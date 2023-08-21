//
// Created by S.Morgenthaler on 08/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTES_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTES_H

#include "process/mesh/gpuGeometryBuffers/VertexAttribute.h"
#include "process/mesh/gpuGeometryBuffers/VertexAttributeVec3.h"
#include "process/mesh/gpuGeometryBuffers/VertexAttributeShort.h"
#include "process/mesh/gpuGeometryBuffers/VertexAttributeFloat.h"
#include "process/mesh/gpuGeometryBuffers/VertexAttributeVec2.h"
#include "process/mesh/gpuGeometryBuffers/VertexAttributeUnsignedByte.h"


class VertexAttributes {

public:

    explicit VertexAttributes(
        vecVertexAttributeVec3_uptr attributesVec3,
        vecVertexAttributeVec2_uptr attributesVec2,
        vecVertexAttributeFloat_uptr attributesFloat,
        vecVertexAttributeShort_uptr attributesShort,
        vecVertexAttributeUnsignedByte_uptr attributesUnsignedByte
    );

    /**
     * Move other vertex attributes into this one.
     * For two Vertex attributes:
     * this =(VA1 (a,b,c,d), VA2 (e,f,g,h)), other = (VA1 (i,j,k,l), VA2 (m,n,o,p))
     * The concatenation gives this result:
     * this = (VA1 (a,b,c,d), VA2 (e,f,g,h)), VA3 (i,j,k,l), VA4 (m,n,o,p)), other = ()
     */
    void operator +=(VertexAttributes&& other);

    /**
     * Merge other vertex attributes into this one.
     * For two Vertex attributes:
     * this =(VA1 (a,b,c,d), VA2 (e,f,g,h)), other = (VA1 (i,j,k,l), VA2 (m,n,o,p))
     * The concatenation gives this result:
     * this = (VA1 (a,b,c,d,i,k,l), VA2 (e,f,g,h,m,n,o,p))), other = ()
     */
    void operator *=(VertexAttributes&& other);

    size_t getNumberOfVertices() const;

    static VertexAttributeVec3_uptr genVertexAttribute(std::vector<glm::vec3> &&vertexAttributeData);

    static VertexAttributeVec2_uptr genVertexAttribute(std::vector<glm::vec2> &&vertexAttributeData);

    static VertexAttributeFloat_uptr genVertexAttribute(std::vector<GLfloat> &&vertexAttributeData);

    static VertexAttributeShort_uptr genVertexAttribute(std::vector<GLshort> &&vertexAttributeData);

    static VertexAttributeUnsignedByte_uptr genVertexAttribute(std::vector<GLubyte> &&vertexAttributeData);

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
     * Merge a list of separated vertex attributes that have the same content (cf vec3, vec2, float, int)
     * between two VertexAttributes objects.
     * This method has to be called for each type of vertex attributes.
     * @tparam T The type of vertex attributes.
     * @param own The vertex attributes list that is the target of merging.
     * @param other The vertex attributes list that will be move into the own one.
     */
    template<typename T>
    void mergeVertexAttributes(T &own, T &&other);

    /**
     * Concat a list of seperated vertex attributes that have the same content (cf vec3, vec2, float, int)
     * between two VertexAttributes objects
     * @tparam T The type of vertex attributes.
     * @param own The vertex attributes list that is the target of concatenation.
     * @param other The vertex attributes list that will be move into the own one.
     */
    template<typename T>
    void concatVertexAttributes(std::vector<T> &own, std::vector<T> &&other);


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

    vecVertexAttributeShort_uptr &&moveAttributesShort();

    vecVertexAttributeUnsignedByte_uptr &&moveAttributesUnsignedByte();

    vecVertexAttributeVec3_uptr _attributesVec3;
    vecVertexAttributeVec2_uptr _attributesVec2;
    vecVertexAttributeFloat_uptr _attributesFloat;
    vecVertexAttributeShort_uptr _attributesShort;
    vecVertexAttributeUnsignedByte_uptr _attributesUnsignedByte;
};

template<typename T>
void VertexAttributes::gatherVertexAttributes(
    vecVertexAttributeBase_uptr &current,
    std::vector<std::unique_ptr<T>> &&vertexAttributes
) {
    current.insert(
        current.end(),
        std::make_move_iterator(vertexAttributes.begin()),
        std::make_move_iterator(vertexAttributes.end())
    );
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

template<typename T>
void VertexAttributes::mergeVertexAttributes(T &own, T &&other) {
    for (size_t i = 0; i < own.size(); ++i) {
        own[i]->merge(std::move(*other[i]));
    }
}

template<typename T>
void VertexAttributes::concatVertexAttributes(std::vector<T> &own, std::vector<T> &&other) {
    for(auto& vertexAttribute: other){
        own.push_back(std::forward<T>(vertexAttribute));
    }
}



#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTES_H
