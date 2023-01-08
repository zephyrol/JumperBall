//
// Created by Sebastien Morgenthaler on 04/01/2023.
//

#include "VertexAttributeBase.h"

VertexAttributeBase::VertexAttributeBase(GLenum dataType) :
    _dataType(dataType) {

}

GLenum VertexAttributeBase::getDataType() const {
    return _dataType;
}

template<typename T>
std::vector<std::unique_ptr<T>>
VertexAttributeBase::filterUnused(
    std::vector<std::unique_ptr<T>> &current,
    const std::function<std::unique_ptr<T>()> &vertexAttributeGenerationFunction
) {
    auto vertexAttribute = vertexAttributeGenerationFunction();
    if (vertexAttribute != nullptr) {
        current.emplace_back(std::move(vertexAttribute));
    }
    return std::move(current);
}

template<typename T>
std::vector<std::unique_ptr<T>> VertexAttributeBase::genAndFilter(
    const std::vector<std::function<std::unique_ptr<T>()>> &vertexAttributeGenerationFunctions
) {
    return std::accumulate(
        vertexAttributeGenerationFunctions.begin(),
        vertexAttributeGenerationFunctions.end(),
        std::vector<std::unique_ptr<T>>{},
        VertexAttributeBase::filterUnused<T>
    );
}

