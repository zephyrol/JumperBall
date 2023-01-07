//
// Created by Sebastien Morgenthaler on 03/01/2023.
//

#include <numeric>
#include "MeshGeometry.h"

MeshGeometry::MeshGeometry(const CstDisplayable_sptr &displayable, const vecCstGeometricShape_sptr &shapes) :
    _vertexAttributes(genVertexAttributes(displayable, shapes)),
    _indices(genIndices(shapes, _vertexAttributes)) {
}

template<typename RawType, typename OpenGLType>
void MeshGeometry::convertAttributesToOpenGLFormat(
    const Displayable::StaticValues<RawType> &rawValues,
    std::vector<OpenGLType> &openGLValues
) {
    for (const RawType &rawValue: rawValues) {
        const OpenGLType openGLValue = Utility::convertToOpenGLFormat(rawValue);
        openGLValues.push_back(openGLValue);
    }
}

void MeshGeometry::merge(MeshGeometry &&other) {

    // Number of positions corresponds to the number of vertices.
    const auto indicesOffset = static_cast<GeometricShape::IndexType>(
        _vertexAttributes.front()->dataLength()
    );

    vecVertexAttributeBase_uptr vertexAttributesOther(other.moveVertexAttributes());
    for(size_t i = 0; i < _vertexAttributes.size(); ++i) {
        _vertexAttributes[i]->merge(std::move(vertexAttributesOther[i]));
    }

    GeometricShape::IndicesBuffer indicesOther(other.moveIndices());
    for(auto& index: indicesOther) {
        index += indicesOffset;
    }
}

vecVertexAttributeBase_uptr &&MeshGeometry::moveVertexAttributes() {
    return std::move(_vertexAttributes);
}

GeometricShape::IndicesBuffer &&MeshGeometry::moveIndices() {
    return std::move(_indices);
}

vecVertexAttributeBase_uptr MeshGeometry::genVertexAttributes(
    const CstDisplayable_sptr &displayable,
    const vecCstGeometricShape_sptr &shapes
) {
    vecVertexAttributeBase_uptr verticesAttributes{};

    // 1. Merging the shapes vertex attributes together.
    const auto mergeVertexAttributes = [](
        vecVertexAttributeBase_uptr &current,
        const CstGeometricShape_sptr &shape
    ) {
        auto shapeVertexAttributes = shape->genVertexAttributes();
        for (size_t i = 0; i < current.size(); ++i) {
            current[i]->merge(std::move(shapeVertexAttributes[i]));
        }
        return std::move(current);
    };

    auto vertexAttributes = std::accumulate(
        shapes.begin() + 1,
        shapes.end(),
        vecVertexAttributeBase_uptr{shapes.front()->genVertexAttributes()},
        mergeVertexAttributes
    );

    // 2. Generate states vertex attributes.
    const std::vector<std::function<VertexAttributeBase_uptr()> > vertexAttributeGenerationFunctions{
        [&displayable]() {
            return genStaticVertexAttribute<decltype(
            Utility::convertToOpenGLFormat(displayable->getStaticIntValues().front())
            )>(displayable->getStaticIntValues(), GL_INT);
        },
        [&displayable]() {
            return genStaticVertexAttribute<decltype(
            Utility::convertToOpenGLFormat(displayable->getStaticFloatValues().front())
            )>(displayable->getStaticFloatValues(), GL_FLOAT);
        },
        [&displayable]() {
            return genStaticVertexAttribute<decltype(
            Utility::convertToOpenGLFormat(displayable->getStaticVec2fValues().front())
            )>(displayable->getStaticVec2fValues(), GL_FLOAT);
        },
        [&displayable]() {
            return genStaticVertexAttribute<decltype(
            Utility::convertToOpenGLFormat(displayable->getStaticVec3fValues().front())
            )>(displayable->getStaticVec3fValues(), GL_FLOAT);
        }
    };

    auto stateVertexAttributes = VertexAttributeBase::genAndFilterVertexAttributes(
        vertexAttributeGenerationFunctions
    );

    // Concatenate shape and state vertex attributes.
    vertexAttributes.insert(
        stateVertexAttributes.begin(),
        std::make_move_iterator(stateVertexAttributes.begin()),
        std::make_move_iterator(stateVertexAttributes.end())
        );
    return vertexAttributes;
}

GeometricShape::IndicesBuffer MeshGeometry::genIndices(
    const vecCstGeometricShape_sptr &shapes,
    const vecVertexAttributeBase_uptr &vertexAttributes
) {
    const auto getNumberOfVertices = [](const VertexAttributeBase_uptr &vertexAttribute) {
        return static_cast<GeometricShape::IndexType>(vertexAttribute->dataLength()) ;
    };

    // The first vertex attribute is always the position. The number of vertices is the data length.
    auto indicesOffset = getNumberOfVertices(vertexAttributes.front());

    auto indices = shapes.front()->genIndices();
    for (auto it = shapes.begin() + 1; it != shapes.end(); ++it) {
        auto shapeIndices = (*it)->genIndices();

        for (auto &index: shapeIndices) {
            index += indicesOffset;
        }

        const auto treatedShapes = it - shapes.begin();
        indicesOffset += getNumberOfVertices(vertexAttributes.at(treatedShapes));
        indices.insert(
            indices.end(),
            std::make_move_iterator(shapeIndices.begin()),
            std::make_move_iterator(shapeIndices.end())
        );
    }

    return indices;
}

template<typename OpenGLType, typename RawType>
VertexAttribute_uptr<OpenGLType> MeshGeometry::genStaticVertexAttribute(
    const Displayable::StaticValues<RawType> &staticVertexAttributeData,
    GLenum glTypeEnum
) {
    // Return null if the vertex attribute does not need to be created.
    if (staticVertexAttributeData.empty()) {
        return nullptr;
    }

    // Convert data to open gl format
    std::vector<OpenGLType> openGLVertexAttributeData;
    for (const auto &rawValue: staticVertexAttributeData) {
        openGLVertexAttributeData.emplace_back(Utility::convertToOpenGLFormat(rawValue));
    }

    // Create the vertex attribute
    return std::unique_ptr<VertexAttribute<OpenGLType>>(
        new VertexAttribute<OpenGLType>(std::move(openGLVertexAttributeData), glTypeEnum)
    );
}

