//
// Created by Sebastien Morgenthaler on 04/01/2023.
//

#ifndef JUMPERBALLAPPLICATION_VERTEXATTRIBUTE_H
#define JUMPERBALLAPPLICATION_VERTEXATTRIBUTE_H


#include "VertexAttributeBase.h"

template<class T>
class VertexAttribute;

template<typename T>
using VertexAttribute_uptr = std::unique_ptr<VertexAttribute<T>>;

template<typename T>
using CstVertexAttribute_uptr = std::unique_ptr<const VertexAttribute<T>>;

template<typename T>
using vecVertexAttribute_uptr = std::vector<VertexAttribute_uptr<T>>;

template<typename T>
using VertexAttribute_sptr = std::shared_ptr<VertexAttribute<T>>;

template<typename T>
using CstVertexAttribute_sptr = std::shared_ptr<const VertexAttribute<T>>;


template<class T>
class VertexAttribute : public VertexAttributeBase {

public:
    explicit VertexAttribute(std::vector<T> &&data, GLenum dataType);

    size_t dataLength() const override;

    void createDataOnGpu() const override;

    std::vector<T> &getDataRef();

    /**
     * Merge a vertex attribute geometry into the current one moving it.
     * @param other The other that will be moved into the current one.
     */
    void merge(VertexAttribute<T> &&other);

protected:
    /**
     * The data contained in a VBO is a list of T values (one T value per Vertex)
     */
    std::vector<T> _data;

private:
    /**
     * Binary operation function usually used in reduce function to filter the unused vertex attributes
     */
    static vecVertexAttribute_uptr<T> filterUnused(
        vecVertexAttribute_uptr<T> &current,
        const std::function<VertexAttribute_uptr<T>()> &vertexAttributeGenerationFunction
    );

};

template<class T>
vecVertexAttribute_uptr<T> VertexAttribute<T>::filterUnused(
    vecVertexAttribute_uptr<T> &current,
    const std::function<VertexAttribute_uptr<T>()> &vertexAttributeGenerationFunction
) {
    auto vertexAttribute = vertexAttributeGenerationFunction();
    if (vertexAttribute != nullptr) {
        current.emplace_back(std::move(vertexAttribute));
    }
    return std::move(current);
}

template<class T>
VertexAttribute<T>::VertexAttribute(std::vector<T> &&data, GLenum dataType):
    VertexAttributeBase(dataType),
    _data(std::move(data)) {
}

template<class T>
void VertexAttribute<T>::createDataOnGpu() const {
    glBufferData(GL_ARRAY_BUFFER, _data.size() * sizeof(T), _data.data(), GL_STATIC_DRAW);
}

template<class T>
size_t VertexAttribute<T>::dataLength() const {
    return _data.size();
}

template<class T>
void VertexAttribute<T>::merge(VertexAttribute<T> &&other) {
    auto &otherData = other.getDataRef();
    _data.insert(
        _data.end(),
        std::make_move_iterator(otherData.begin()),
        std::make_move_iterator(otherData.end())
    );
}

template<class T>
std::vector<T> &VertexAttribute<T>::getDataRef() {
    return _data;
}

#endif //JUMPERBALLAPPLICATION_VERTEXATTRIBUTE_H
