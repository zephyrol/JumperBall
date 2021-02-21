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

template<typename T> void Mesh::concatVector (std::vector <T>& current, const std::vector <T>& other) {
    current.insert(current.end(), other.begin(), other.end());
}

void Mesh::concatIndices(
  std::vector <GLushort>& currentIndices,
  const std::vector <GLushort>& otherIndices,
  size_t offset
  ) {
    std::vector <GLushort> newIndices = otherIndices;
    for (GLushort& newIndice : newIndices) {
        newIndice += static_cast <GLushort>(offset);
    }
    concatVector(currentIndices, newIndices);
}

template<typename T> void Mesh::concatIndependantStaticAttribute (std::vector <T>& staticAttributeData,
                                                       const std::shared_ptr <const std::vector <T> >& shapeData)
{
    if (shapeData) {
        concatVector(staticAttributeData, *shapeData);
    }
}

void Mesh::concatIndicesStaticAttribute (std::vector <GLushort>& staticAttributeIndices,
                                                       const std::shared_ptr <const std::vector <GLushort> >& shapeIndices,
                                                       size_t offset)
{
    if (shapeIndices) {
        concatIndices(staticAttributeIndices, *shapeIndices, offset);
    }
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

    concatIndices(staticAttributes.indices, other.indices, staticAttributes.positions.size());
    concatVector(staticAttributes.positions, other.positions);
    concatVector(staticAttributes.normals, other.normals);
    concatVector(staticAttributes.colors, other.colors);
    concatVector(staticAttributes.uvCoords, other.uvCoords);

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
    size_t numberOfVertices = 0;
    for (const auto& shape : _shapes) {
        numberOfVertices += shape->numberOfVertices();
    }
    return numberOfVertices;
}

template<> Mesh::StaticAttributes Mesh::genAttributes <Mesh::StaticAttributes>() const {
    Mesh::StaticAttributes staticAttributes;
    for (const CstGeometricShape_sptr& shape : _shapes) {
        concatIndicesStaticAttribute(staticAttributes.indices, shape->indices(), staticAttributes.positions.size());
        concatIndependantStaticAttribute(staticAttributes.indices, shape->indices());
        concatIndependantStaticAttribute(staticAttributes.positions, shape->positions());
        concatIndependantStaticAttribute(staticAttributes.normals, shape->normals());
        concatIndependantStaticAttribute(staticAttributes.colors, shape->colors());
        concatIndependantStaticAttribute(staticAttributes.uvCoords, shape->uvCoords());
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
