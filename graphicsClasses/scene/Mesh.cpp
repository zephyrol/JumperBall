/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Mesh.h"

Mesh::Mesh(const State& state, const vecCstGeometricShape_sptr& shapes):
    _state(state),
    _shapes(shapes),
    _numberOfVertices(computeNumberOfVertices()) {
}

void Mesh::update() {

}

size_t Mesh::numberOfVertices() const {
    return _numberOfVertices;
}

template<typename T> void Mesh::concatVector (std::vector <T>& current, const std::vector <T>& data) {
    current.insert(current.end(), data.begin(), data.end());
}

template<typename T> void Mesh::duplicateDynamicAttribute (std::vector <std::vector <T> >& attributes,
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

Mesh::StaticAttributes Mesh::concatAttributes (const Mesh::StaticAttributes& current,
                                               const Mesh::StaticAttributes& other) {
    Mesh::StaticAttributes staticAttributes = current;
    concatVector(staticAttributes.positions, other.positions);
    concatVector(staticAttributes.normals, other.normals);
    concatVector(staticAttributes.colors, other.colors);
    concatVector(staticAttributes.uvCoords, other.uvCoords);

    std::vector <GLushort> newIndices = other.indices;
    for (GLushort& newIndice : newIndices) {
        newIndice += static_cast <GLushort>(staticAttributes.positions.size());
    }
    concatVector(staticAttributes.indices, newIndices);
    return staticAttributes;
}

Mesh::DynamicAttributes Mesh::concatAttributes (const Mesh::DynamicAttributes& current,
                                                const Mesh::DynamicAttributes& other) {
    Mesh::DynamicAttributes dynamicAttributes = current;
    concatVector(dynamicAttributes.dynamicFloats, other.dynamicFloats);
    concatVector(dynamicAttributes.dynamicsVec2s, other.dynamicsVec2s);
    concatVector(dynamicAttributes.dynamicsVec3s, other.dynamicsVec3s);
    concatVector(dynamicAttributes.dynamicUbytes, other.dynamicUbytes);
    return dynamicAttributes;
}

size_t Mesh::computeNumberOfVertices() const {
    size_t _numberOfVertices = 0;
    for (const auto& shape : _shapes) {
        _numberOfVertices += shape->numberOfVertices();
    }
    return _numberOfVertices;
}

template<> Mesh::StaticAttributes Mesh::genAttributes <Mesh::StaticAttributes>() const {
    Mesh::StaticAttributes staticAttributes;
    for (const CstGeometricShape_sptr& shape : _shapes) {
        concatVector(staticAttributes.positions, *shape->positions());
        concatVector(staticAttributes.normals, *shape->normals());
        concatVector(staticAttributes.colors, *shape->colors());
        concatVector(staticAttributes.uvCoords, *shape->uvCoords());
        concatVector(staticAttributes.indices, *shape->indices());
    }
    return staticAttributes;
}

template<> Mesh::DynamicAttributes Mesh::genAttributes <Mesh::DynamicAttributes>() const {
    Mesh::DynamicAttributes dynamicAttributes;
    std::vector <glm::vec3> glmVec3s {};
    std::vector <glm::vec2> glmVec2s {};
    std::vector <GLfloat> glFloats {};
    convertAttributesToOpenGLFormat(_state.getDynamicFloats(), glFloats);
    convertAttributesToOpenGLFormat(_state.getDynamicVec2fs(), glmVec2s);
    convertAttributesToOpenGLFormat(_state.getDynamicVec3fs(), glmVec3s);
    duplicateDynamicAttribute(dynamicAttributes.dynamicFloats, glFloats);
    duplicateDynamicAttribute(dynamicAttributes.dynamicsVec2s, glmVec2s);
    duplicateDynamicAttribute(dynamicAttributes.dynamicsVec3s, glmVec3s);
    return dynamicAttributes;
}

Mesh::Uniforms Mesh::genUniformsValues() const {
    Mesh::Uniforms uniforms;
    std::map <std::string, glm::vec3> glmVec3s {};
    std::map <std::string, glm::vec2> glmVec2s {};
    std::map <std::string, GLfloat> glFloats {};
    convertUniformsToOpenGLFormat(_state.getStaticFloatValues(), glFloats);
    convertUniformsToOpenGLFormat(_state.getStaticVec2fValues(), glmVec2s);
    convertUniformsToOpenGLFormat(_state.getStaticVec3fValues(), glmVec3s);
    uniforms.uniformFloats = glFloats;
    uniforms.uniformVec2s = glmVec2s;
    uniforms.uniformVec3s = glmVec3s;
    return uniforms;
}
