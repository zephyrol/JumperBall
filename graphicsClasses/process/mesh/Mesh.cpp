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
}

Displayable::GlobalState Mesh::getGlobalState() const {
    return _displayable->getGlobalState();
}

Mesh::UniformsValues Mesh::genUniformsValues() const {
    Mesh::UniformsValues uniforms;
    convertUniformsToOpenGLFormat(_displayable->getDynamicIntValues(), uniforms.uniformInts);
    convertUniformsToOpenGLFormat(_displayable->getDynamicFloatValues(), uniforms.uniformFloats);
    convertUniformsToOpenGLFormat(_displayable->getDynamicVec2fValues(), uniforms.uniformVec2s);
    convertUniformsToOpenGLFormat(_displayable->getDynamicVec3fValues(), uniforms.uniformVec3s);
    convertUniformsToOpenGLFormat(_displayable->getDynamicQuaternionValues(), uniforms.uniformVec4s);
    return uniforms;
}

MeshGeometry Mesh::genMeshGeometry() const {
    return MeshGeometry::createInstance(_displayable, _shapes);
}

bool Mesh::updatingIsUseless() const {
    return _updatingIsUseless;
}

Mesh::UniformsNames Mesh::genUniformsNames() const {
    Mesh::UniformsNames uniformsNames;
    uniformsNames.uniformsInts = _displayable->getDynamicIntNames();
    uniformsNames.uniformFloats = _displayable->getDynamicFloatNames();
    uniformsNames.uniformVec2s = _displayable->getDynamicVec2fNames();
    uniformsNames.uniformVec3s = _displayable->getDynamicVec3fNames();
    uniformsNames.uniformVec4s = _displayable->getDynamicQuaternionNames();
    return uniformsNames;
}

std::vector<std::string> Mesh::genGatheredUniformsNames() const {
    auto uniformNames = genUniformsNames();
    std::vector<std::string> gatheredUniformNames (std::move(uniformNames.uniformsInts));
    gatheredUniformNames.insert(
        gatheredUniformNames.end(),
        std::make_move_iterator(uniformNames.uniformFloats.begin()),
        std::make_move_iterator(uniformNames.uniformFloats.end())
        );
    gatheredUniformNames.insert(
        gatheredUniformNames.end(),
        std::make_move_iterator(uniformNames.uniformVec2s.begin()),
        std::make_move_iterator(uniformNames.uniformVec2s.end())
    );
    gatheredUniformNames.insert(
        gatheredUniformNames.end(),
        std::make_move_iterator(uniformNames.uniformVec3s.begin()),
        std::make_move_iterator(uniformNames.uniformVec3s.end())
    );
    gatheredUniformNames.insert(
        gatheredUniformNames.end(),
        std::make_move_iterator(uniformNames.uniformVec4s.begin()),
        std::make_move_iterator(uniformNames.uniformVec4s.end())
    );
    return gatheredUniformNames;
}
