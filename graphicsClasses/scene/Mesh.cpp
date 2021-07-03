/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Mesh.h"

Mesh::Mesh(Frames_uptr <ObjectState>&& frames, vecCstGeometricShape_sptr&& shapes):
    _frames(std::move(frames)),
    _shapes(std::move(shapes)),
    _numberOfVertices(computeNumberOfVertices()) {
}

void Mesh::swapFrames() {
    _frames->swapFrames();
}

ObjectState::GlobalState Mesh::update() {
    auto updatableFrame = _frames->getUpdatableState();
    return updatableFrame->update();
}

size_t Mesh::numberOfVertices() const {
    return _numberOfVertices;
}

template<typename T> void Mesh::duplicateStateVertexAttribute (std::vector <std::vector <T> >& attributes,
                                                               const std::vector <T>& values) const {
    for (const T& value : values) {
        attributes.push_back(std::vector <T>(_numberOfVertices, value));
    }
}

template<typename RawType, typename OpenGLType> void Mesh::convertAttributesToOpenGLFormat (
    const ObjectState::StaticValues <RawType>& rawValues,
    std::vector <OpenGLType>& openGLValues) {
    for (const RawType& rawValue : rawValues) {
        const OpenGLType openGLValue = Utility::convertToOpenGLFormat(rawValue);
        openGLValues.push_back(openGLValue);
    }
}

template<typename RawType, typename OpenGLType> void Mesh::convertUniformsToOpenGLFormat (
    const ObjectState::DynamicValues <RawType>& rawValues,
    Mesh::UniformVariables <OpenGLType>& openGLValues) {
    for (const auto& rawValue : rawValues) {
        const OpenGLType openGLValue = Utility::convertToOpenGLFormat(rawValue.second);
        openGLValues[rawValue.first] = openGLValue;
    }
}

size_t Mesh::computeNumberOfVertices() const {
    size_t numberOfVertices = 0;
    for (const auto& shape : _shapes) {
        numberOfVertices += shape->numberOfVertices();
    }
    return numberOfVertices;
}

Mesh::Uniforms Mesh::genUniformsValues() const {
    Mesh::Uniforms uniforms;
    const auto renderableState = _frames->getRenderableState();
    convertUniformsToOpenGLFormat(renderableState->getDynamicFloats(), uniforms.uniformFloats);
    convertUniformsToOpenGLFormat(renderableState->getDynamicVec2fs(), uniforms.uniformVec2s);
    convertUniformsToOpenGLFormat(renderableState->getDynamicVec3fs(), uniforms.uniformVec3s);
    convertUniformsToOpenGLFormat(renderableState->getDynamicQuaternions(), uniforms.uniformVec4s);
    return uniforms;
}

Mesh::MeshVerticesInfo Mesh::genMeshVerticesInfo() const {

    GeometricShape::ShapeVerticesInfo gatheredShapeVerticesInfo;
    for (const CstGeometricShape_sptr& shape : _shapes) {
        GeometricShape::concatShapeVerticesInfo(gatheredShapeVerticesInfo, shape->genShapeVerticesInfo());
    }

    Mesh::StateVertexAttributes stateVertexAttributes;
    std::vector <glm::vec3> glmVec3s {};
    std::vector <glm::vec2> glmVec2s {};
    std::vector <GLfloat> glFloats {};

    const auto renderableState = _frames->getRenderableState();
    convertAttributesToOpenGLFormat(renderableState->getStaticFloatValues(), glFloats);
    convertAttributesToOpenGLFormat(renderableState->getStaticVec2fValues(), glmVec2s);
    convertAttributesToOpenGLFormat(renderableState->getStaticVec3fValues(), glmVec3s);

    duplicateStateVertexAttribute(stateVertexAttributes.staticFloats, glFloats);
    duplicateStateVertexAttribute(stateVertexAttributes.staticVec2s, glmVec2s);
    duplicateStateVertexAttribute(stateVertexAttributes.staticVec3s, glmVec3s);

    Mesh::MeshVerticesInfo meshVerticesInfo;
    meshVerticesInfo.shapeVerticesInfo = gatheredShapeVerticesInfo;
    meshVerticesInfo.stateVertexAttributes = stateVertexAttributes;

    return meshVerticesInfo;
}


template<typename T> void Mesh::concatStateVertexAttribute (std::vector <std::vector <T> >& current,
                                                            const std::vector <std::vector <T> >& other) {
    for (size_t i = 0; i < other.size(); ++i) {
        if (i >= current.size()) {
            current.push_back({});
        }
        Utility::concatVector(current.at(i), other.at(i));
    }
}

void Mesh::concatStateVertexAttributes (StateVertexAttributes& current, const StateVertexAttributes& other) {
    concatStateVertexAttribute(current.staticFloats, other.staticFloats);
    concatStateVertexAttribute(current.staticVec2s, other.staticVec2s);
    concatStateVertexAttribute(current.staticVec3s, other.staticVec3s);
}

void Mesh::concatMeshVerticesInfo (Mesh::MeshVerticesInfo& current,
                                   const Mesh::MeshVerticesInfo& other) {
    GeometricShape::concatShapeVerticesInfo(current.shapeVerticesInfo, other.shapeVerticesInfo);

    Mesh::StateVertexAttributes& currentStateVertexAttributes = current.stateVertexAttributes;
    const Mesh::StateVertexAttributes& otherStateVertexAttributes = other.stateVertexAttributes;
    concatStateVertexAttributes(currentStateVertexAttributes, otherStateVertexAttributes);
}
