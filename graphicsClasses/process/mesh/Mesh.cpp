/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Mesh.h"

#include <utility>

Mesh::Mesh(CstDisplayable_sptr displayable, vecCstGeometricShape_sptr&& shapes, short dynamicsId)
    : _displayable(std::move(displayable)), _shapes(std::move(shapes)), _dynamicsId(dynamicsId) {}

MeshUniforms Mesh::genMeshUniforms(const ShaderProgram_uptr& shaderProgram) const {
    return MeshUniforms::createInstance(_displayable, shaderProgram, _dynamicsId);
}

MeshGeometry Mesh::genMeshGeometry() const {
    return MeshGeometry::createInstance(_displayable, _shapes, _dynamicsId);
}
