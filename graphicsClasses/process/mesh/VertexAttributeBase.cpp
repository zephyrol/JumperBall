//
// Created by Sebastien Morgenthaler on 04/01/2023.
//

#include <numeric>
#include "VertexAttributeBase.h"

VertexAttributeBase::VertexAttributeBase(GLenum dataType):
_dataType(dataType){

}

GLenum VertexAttributeBase::getDataType() const {
    return _dataType;
}

void VertexAttributeBase::merge(VertexAttributeBase_uptr &&other) {
}

vecVertexAttributeBase_uptr VertexAttributeBase::filterUnusedAttributes(
    vecVertexAttributeBase_uptr &current,
    const std::function<VertexAttributeBase_uptr()> &vertexAttributeGenerationFunction
) {
    auto vertexAttribute = vertexAttributeGenerationFunction();
    if (vertexAttribute != nullptr) {
        current.emplace_back(std::move(vertexAttribute));
    }
    return std::move(current);
}

vecVertexAttributeBase_uptr VertexAttributeBase::genAndFilterVertexAttributes(
    const std::vector<std::function<VertexAttributeBase_uptr()> > &vertexAttributeGenerationFunctions
) {
    return std::accumulate(
        vertexAttributeGenerationFunctions.begin(),
        vertexAttributeGenerationFunctions.end(),
        vecVertexAttributeBase_uptr{},
        VertexAttributeBase::filterUnusedAttributes
    );
}
