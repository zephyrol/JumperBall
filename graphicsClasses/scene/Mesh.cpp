/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Mesh.h"

Mesh::Mesh(const State& state, const vecCstGeometricShape_sptr& shapes):
    _state(state),
    _shapes(shapes) {
}

void Mesh::update() {
  
}

template<typename T> void Mesh::concatShapeData (std::vector <T>& current,
                                                 const std::vector <T>& dataShape) {
    current.insert(current.end(), dataShape.begin(), dataShape.end());
}

template<typename Raw, typename OpenGL>
void Mesh::convertVectorToOpenGLFormat(const std::vector<Raw>& rawValues, std::vector<OpenGL>& openGLValues) 
{
  for (const Raw& rawValue : rawValues) {
    const OpenGL openGLValue = Utility::convertToOpenGLFormat(rawValue);
    openGLValues.push_back(openGLValue);
  }
}

Mesh::StaticAttributes Mesh::concatStaticAttributes (const Mesh::StaticAttributes& current,
                                                     const Mesh::StaticAttributes& other) {
    Mesh::StaticAttributes staticAttributes = current;
    concatShapeData(staticAttributes.positions, other.positions);
    concatShapeData(staticAttributes.normals, other.normals);
    concatShapeData(staticAttributes.colors, other.colors);
    concatShapeData(staticAttributes.uvCoords, other.uvCoords);

    std::vector <GLushort> newIndices = other.indices;
    for (GLushort& newIndice : newIndices) {
        newIndice += static_cast <GLushort>(staticAttributes.positions.size());
    }
    concatShapeData(staticAttributes.indices, newIndices);
    return staticAttributes;
}

Mesh::StaticAttributes Mesh::genStaticAttributes() const {
    Mesh::StaticAttributes staticAttributes;
    for (const CstGeometricShape_sptr& shape : _shapes) {
        concatShapeData(staticAttributes.positions, *shape->positions());
        concatShapeData(staticAttributes.normals, *shape->normals());
        concatShapeData(staticAttributes.colors, *shape->colors());
        concatShapeData(staticAttributes.uvCoords, *shape->uvCoords());
        concatShapeData(staticAttributes.indices, *shape->indices());
    }
    return staticAttributes;
}

Mesh::DynamicAttributes Mesh::genDynamicAttributes() const {
  Mesh::DynamicAttributes dynamicAttributes;
    for (const CstGeometricShape_sptr& shape : _shapes) {
        //const std::vector<Glubyte> a = _state.getDynamicUChars();
        std::vector<glm::vec3> glmVec3s {};
        convertVectorToOpenGLFormat( _state.getDynamicVec3fs(), glmVec3s);
        //const std::vector<Glfloat> c = _state.getDynamicVec2fs();
        //const std::vector<Glfloat> c = _state.getDynamicVec2fs();
        //concatShapeData(dynamicAttributes.dynamicUbytes, convertVectorToOpenGLFormat(_state.getDynamicUChars()));
        //concatShapeData(dynamicAttributes.dynamicFloats, convertVectorToOpenGLFormat(b));
        //concatShapeData(dynamicAttributes.dynamicsVec2s, convertVectorToOpenGLFormat(_state.getDynamicVec2fs()));
        //concatShapeData(dynamicAttributes.dynamicsVec3s, convertVectorToOpenGLFormat(_state.getDynamicVec3fs()));
    }
    return dynamicAttributes;
}
