/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Mesh.h"

Mesh::Mesh(const std::shared_ptr<const Displayable> &displayable, vecCstGeometricShape_sptr &&shapes) :
    _displayable(displayable),
    _shapes(std::move(shapes)),
    _numberOfVertices(computeNumberOfVertices()),
    _updatingIsUseless(!_displayable->globalStateMayChange()) {
}

Displayable::GlobalState Mesh::getGlobalState() const {
    return _displayable->getGlobalState();
}

size_t Mesh::numberOfVertices() const {
    return _numberOfVertices;
}

template<typename RawType, typename OpenGLType>
void Mesh::convertUniformsToOpenGLFormat(
    const Displayable::DynamicValues<RawType> &rawValues,
    Mesh::UniformVariables<OpenGLType> &openGLValues) {
    for (const auto &rawValue: rawValues) {
        const OpenGLType openGLValue = Utility::convertToOpenGLFormat(rawValue.second);
        openGLValues[rawValue.first] = openGLValue;
    }
}

size_t Mesh::computeNumberOfVertices() const {
    size_t numberOfVertices = 0;
    for (const auto &shape: _shapes) {
        numberOfVertices += shape->numberOfVertices();
    }
    return numberOfVertices;
}

Mesh::Uniforms Mesh::genUniformsValues() const {
    Mesh::Uniforms uniforms;
    convertUniformsToOpenGLFormat(_displayable->getDynamicInts(), uniforms.uniformInts);
    convertUniformsToOpenGLFormat(_displayable->getDynamicFloats(), uniforms.uniformFloats);
    convertUniformsToOpenGLFormat(_displayable->getDynamicFloats(), uniforms.uniformFloats);
    convertUniformsToOpenGLFormat(_displayable->getDynamicVec2fs(), uniforms.uniformVec2s);
    convertUniformsToOpenGLFormat(_displayable->getDynamicVec3fs(), uniforms.uniformVec3s);
    convertUniformsToOpenGLFormat(_displayable->getDynamicQuaternions(), uniforms.uniformVec4s);
    return uniforms;
}

Mesh::MeshGeometry Mesh::genMeshGeometry() const {

    GeometricShape::ShapeVerticesInfo gatheredShapeVerticesInfo;
    for (const CstGeometricShape_sptr &shape: _shapes) {
        GeometricShape::concatShapeVerticesInfo(gatheredShapeVerticesInfo, shape->genShapeVerticesInfo());
    }

    Mesh::StateVertexAttributes stateVertexAttributes;
    std::vector<glm::vec3> glmVec3s{};
    std::vector<glm::vec2> glmVec2s{};
    std::vector<GLfloat> glFloats{};
    std::vector<GLint> glInts{};

    convertAttributesToOpenGLFormat(_displayable->getStaticIntValues(), glInts);
    convertAttributesToOpenGLFormat(_displayable->getStaticFloatValues(), glFloats);
    convertAttributesToOpenGLFormat(_displayable->getStaticVec2fValues(), glmVec2s);
    convertAttributesToOpenGLFormat(_displayable->getStaticVec3fValues(), glmVec3s);

    duplicateStateVertexAttribute(stateVertexAttributes.staticInts, glInts);
    duplicateStateVertexAttribute(stateVertexAttributes.staticFloats, glFloats);
    duplicateStateVertexAttribute(stateVertexAttributes.staticVec2s, glmVec2s);
    duplicateStateVertexAttribute(stateVertexAttributes.staticVec3s, glmVec3s);

    Mesh::MeshGeometry meshGeometry;
    meshGeometry.shapeVertexAttributes = gatheredShapeVerticesInfo.shapeVertexAttributes;
    meshGeometry.stateVertexAttributes = stateVertexAttributes;
    meshGeometry.indices = gatheredShapeVerticesInfo.indices;
    return meshGeometry;
}


void Mesh::concatMeshVerticesInfo(
    Mesh::MeshGeometry &current,
    const Mesh::MeshGeometry &other
) {
    GeometricShape::concatShapeVerticesInfo(current.shapeVerticesInfo, other.shapeVerticesInfo);

    Mesh::StateVertexAttributes &currentStateVertexAttributes = current.stateVertexAttributes;
    const Mesh::StateVertexAttributes &otherStateVertexAttributes = other.stateVertexAttributes;
    concatStateVertexAttributes(currentStateVertexAttributes, otherStateVertexAttributes);
}

bool Mesh::updatingIsUseless() const {
    return _updatingIsUseless;
}
