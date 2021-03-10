/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Mesh.h"

Mesh::Mesh(std::unique_ptr <State>&& state, vecCstGeometricShape_sptr&& shapes):
    _state(std::move(state)),
    _shapes(std::move(shapes)),
    _numberOfVertices(computeNumberOfVertices()) {
}

void Mesh::update() {
    _state->update();
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
    const std::vector <RawType>& rawValues,
    std::vector <OpenGLType>& openGLValues) {
    for (const RawType& rawValue : rawValues) {
        const OpenGLType openGLValue = Utility::convertToOpenGLFormat(rawValue);
        openGLValues.push_back(openGLValue);
    }
}

template<typename RawType, typename OpenGLType> void Mesh::convertUniformsToOpenGLFormat (
    const std::map <std::string, RawType>& rawValues,
    std::map <std::string, OpenGLType>& openGLValues) {
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
    std::map <std::string, glm::vec3> glmVec3s {};
    std::map <std::string, glm::vec2> glmVec2s {};
    std::map <std::string, GLfloat> glFloats {};
    convertUniformsToOpenGLFormat(_state->getDynamicFloats(), glFloats);
    convertUniformsToOpenGLFormat(_state->getDynamicVec2fs(), glmVec2s);
    convertUniformsToOpenGLFormat(_state->getDynamicVec3fs(), glmVec3s);
    uniforms.uniformFloats = glFloats;
    uniforms.uniformVec2s = glmVec2s;
    uniforms.uniformVec3s = glmVec3s;
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
    convertAttributesToOpenGLFormat(_state->getStaticFloatValues(), glFloats);
    convertAttributesToOpenGLFormat(_state->getStaticVec2fValues(), glmVec2s);
    convertAttributesToOpenGLFormat(_state->getStaticVec3fValues(), glmVec3s);
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
