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

template<typename T> void Mesh::concatIndependantShapeVertexAttribute (std::vector <T>& ShapeVertexAttributeData,
                                                       const std::shared_ptr <const std::vector <T> >& shapeData)
{
    if (shapeData) {
        concatVector(ShapeVertexAttributeData, *shapeData);
    }
}

void Mesh::concatIndicesShapeVertexAttribute (std::vector <GLushort>& ShapeVertexAttributeIndices,
                                                       const std::shared_ptr <const std::vector <GLushort> >& shapeIndices,
                                                       size_t offset)
{
    if (shapeIndices) {
        concatIndices(ShapeVertexAttributeIndices, *shapeIndices, offset);
    }
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

Mesh::ShapeVertexAttributes Mesh::concatAttributes (const Mesh::ShapeVertexAttributes& current,
                                               const Mesh::ShapeVertexAttributes& other) {
    Mesh::ShapeVertexAttributes ShapeVertexAttributes = current;

    concatIndices(ShapeVertexAttributes.indices, other.indices, ShapeVertexAttributes.positions.size());
    concatVector(ShapeVertexAttributes.positions, other.positions);
    concatVector(ShapeVertexAttributes.normals, other.normals);
    concatVector(ShapeVertexAttributes.colors, other.colors);
    concatVector(ShapeVertexAttributes.uvCoords, other.uvCoords);

    return ShapeVertexAttributes;
}

Mesh::StateVertexAttributes Mesh::concatAttributes (const Mesh::StateVertexAttributes& current,
                                                const Mesh::StateVertexAttributes& other) {
    Mesh::StateVertexAttributes StateVertexAttributes = current;
    concatVector(StateVertexAttributes.dynamicFloats, other.dynamicFloats);
    concatVector(StateVertexAttributes.dynamicsVec2s, other.dynamicsVec2s);
    concatVector(StateVertexAttributes.dynamicsVec3s, other.dynamicsVec3s);
    concatVector(StateVertexAttributes.dynamicUbytes, other.dynamicUbytes);
    return StateVertexAttributes;
}

size_t Mesh::computeNumberOfVertices() const {
    size_t numberOfVertices = 0;
    for (const auto& shape : _shapes) {
        numberOfVertices += shape->numberOfVertices();
    }
    return numberOfVertices;
}

template<> Mesh::ShapeVertexAttributes Mesh::genAttributes <Mesh::ShapeVertexAttributes>() const {
    Mesh::ShapeVertexAttributes ShapeVertexAttributes;
    for (const CstGeometricShape_sptr& shape : _shapes) {
        concatIndicesShapeVertexAttribute(ShapeVertexAttributes.indices, shape->indices(), ShapeVertexAttributes.positions.size());
        concatIndependantShapeVertexAttribute(ShapeVertexAttributes.indices, shape->indices());
        concatIndependantShapeVertexAttribute(ShapeVertexAttributes.positions, shape->positions());
        concatIndependantShapeVertexAttribute(ShapeVertexAttributes.normals, shape->normals());
        concatIndependantShapeVertexAttribute(ShapeVertexAttributes.colors, shape->colors());
        concatIndependantShapeVertexAttribute(ShapeVertexAttributes.uvCoords, shape->uvCoords());
    }
    return ShapeVertexAttributes;
}

template<> Mesh::StateVertexAttributes Mesh::genAttributes <Mesh::StateVertexAttributes>() const {
    Mesh::StateVertexAttributes StateVertexAttributes;
    std::vector <glm::vec3> glmVec3s {};
    std::vector <glm::vec2> glmVec2s {};
    std::vector <GLfloat> glFloats {};
    convertAttributesToOpenGLFormat(_state.getDynamicFloats(), glFloats);
    convertAttributesToOpenGLFormat(_state.getDynamicVec2fs(), glmVec2s);
    convertAttributesToOpenGLFormat(_state.getDynamicVec3fs(), glmVec3s);
    duplicateStateVertexAttribute(StateVertexAttributes.dynamicFloats, glFloats);
    duplicateStateVertexAttribute(StateVertexAttributes.dynamicsVec2s, glmVec2s);
    duplicateStateVertexAttribute(StateVertexAttributes.dynamicsVec3s, glmVec3s);
    return StateVertexAttributes;
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
