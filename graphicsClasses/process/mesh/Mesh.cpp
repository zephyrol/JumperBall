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

MeshUniforms Mesh::genMeshUniforms(CstShaderProgram_sptr shaderProgram) const {
    return MeshUniforms::createInstance(_displayable, shaderProgram)
}

MeshGeometry Mesh::genMeshGeometry() const {
    return MeshGeometry::createInstance(_displayable, _shapes);
}

bool Mesh::updatingIsUseless() const {
    return _updatingIsUseless;
}

std::vector<std::string> Mesh::genUniformsNames() const {
    _displayable->getDynamicNames();
}
