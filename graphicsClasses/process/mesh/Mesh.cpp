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
    _updatingIsUseless(!_displayable->globalStateMayChange()) {
    if(_shapes[0] == nullptr) {
        std::cout << "null" << std::endl;
    }
}

Displayable::GlobalState Mesh::getGlobalState() const {
    return _displayable->getGlobalState();
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

MeshGeometry Mesh::genMeshGeometry() const {
    return MeshGeometry::createInstance(_displayable, _shapes);
}


// void Mesh::concatMeshVerticesInfo(
//     Mesh::MeshGeometry &current,
//     const Mesh::MeshGeometry &other
// ) {
//     GeometricShape::concatShapeVerticesInfo(current.shapeVerticesInfo, other.shapeVerticesInfo);
//
//     Mesh::StateVertexAttributes &currentStateVertexAttributes = current.stateVertexAttributes;
//     const Mesh::StateVertexAttributes &otherStateVertexAttributes = other.stateVertexAttributes;
//     concatStateVertexAttributes(currentStateVertexAttributes, otherStateVertexAttributes);
// }

bool Mesh::updatingIsUseless() const {
    return _updatingIsUseless;
}
