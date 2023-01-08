//
// Created by Sebastien Morgenthaler on 03/01/2023.
//

#include "MeshGeometry.h"


MeshGeometry::MeshGeometry(
    vecVertexAttributeBase_uptr vertexAttributes,
    GeometricShape::IndicesBuffer indices
) :
    _vertexAttributes(std::move(vertexAttributes)),
    _indices(std::move(indices)) {
}

void MeshGeometry::merge(MeshGeometry &&other) {

    // Number of positions corresponds to the number of vertices.
    const auto indicesOffset = static_cast<GeometricShape::IndexType>(
        _vertexAttributes.front()->dataLength()
    );

    vecVertexAttributeBase_uptr vertexAttributesOther(other.moveVertexAttributes());
    for (size_t i = 0; i < _vertexAttributes.size(); ++i) {
        // VertexAttributeBase::merge(_vertexAttributes[i], std::move(vertexAttributesOther[i]));
    }

    GeometricShape::IndicesBuffer indicesOther(other.moveIndices());
    for (auto &index: indicesOther) {
        index += indicesOffset;
    }
}

vecVertexAttributeBase_uptr &&MeshGeometry::moveVertexAttributes() {
    return std::move(_vertexAttributes);
}

GeometricShape::IndicesBuffer &&MeshGeometry::moveIndices() {
    return std::move(_indices);
}

const GeometricShape::IndicesBuffer &MeshGeometry::indices() const {
    return _indices;
}

const vecVertexAttributeBase_uptr &MeshGeometry::vertexAttributes() {
    return _vertexAttributes;
}

MeshGeometry MeshGeometry::createInstance(
    const CstDisplayable_sptr &displayable,
    const vecCstGeometricShape_sptr &shapes
) {

    const auto &headShape = shapes.front();

    vecVertexAttributeBase_uptr vertexAttributes = headShape->genVertexAttributes();

    GeometricShape::IndicesBuffer indices = headShape->genIndices();
    GeometricShape::IndexType indicesOffset = VertexAttributeBase::getNumberOfVertices(vertexAttributes);

    // 1. Merging the shapes vertex attributes and the concat indices together.
    for (auto it = shapes.begin() + 1; it < shapes.end(); ++it) {
        const auto &shape = *it;

        // Concat indices
        auto shapeIndices = shape->genIndices();
        for (auto &index: shapeIndices) {
            index += indicesOffset;
        }

        // Look the number of vertices on the current shape and increase the offset for the next loop turn.
        auto shapeVertexAttributes = shape->genVertexAttributes();
        indicesOffset += VertexAttributeBase::getNumberOfVertices(shapeVertexAttributes);

        // Merge vertex attributes.
        for (size_t i = 0; i < vertexAttributes.size(); ++i) {
            VertexAttributeBase::merge(vertexAttributes[i], std::move(shapeVertexAttributes[i]));
        }
    }

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

    return MeshGeometry(std::move(vertexAttributes), std::move(indices));
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

