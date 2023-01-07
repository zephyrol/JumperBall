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
using VertexAttribute_sptr = std::shared_ptr<VertexAttribute<T>>;

template<typename T>
using CstVertexAttribute_sptr = std::shared_ptr<const VertexAttribute<T>>;


template<class T>
class VertexAttribute : public VertexAttributeBase {

public:
    explicit VertexAttribute(std::vector<T> &&data, GLenum dataType);

    void merge(VertexAttribute_uptr<T> &&other);

    size_t dataLength() const override;

private:
    std::vector<T> &getDataRef();

    /**
     * The data contained in a VBO is a list of T values (one T value per Vertex)
     */
    std::vector<T> _data;
};

template<class T>
size_t VertexAttribute<T>::dataLength() const {
    return _data.size();
}

template<class T>
VertexAttribute<T>::VertexAttribute(std::vector<T> &&data, GLenum dataType):
    VertexAttributeBase(dataType),
    _data(std::move(data)) {
}

template<class T>
void VertexAttribute<T>::merge(VertexAttribute_uptr<T> &&other) {
    auto &otherData = other->getDataRef();
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
