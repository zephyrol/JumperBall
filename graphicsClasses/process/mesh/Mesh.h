/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */

#ifndef MESH_H
#define MESH_H

#include "ShaderProgram.h"
#include "geometry/GeometricShape.h"
#include "scene/Displayable.h"
#include "MeshGeometry.h"
#include "MeshUniforms.h"

class Mesh;

using Mesh_sptr = std::shared_ptr<Mesh>;
using CstMesh_sptr = std::shared_ptr<const Mesh>;
using vecCstMesh_sptr = std::vector<CstMesh_sptr>;
using vecMesh_sptr = std::vector<Mesh_sptr>;

class Mesh {

public:

    Mesh(
        CstDisplayable_sptr displayable,
        vecCstGeometricShape_sptr &&shapes,
        short dynamicsId
    );

    Mesh(const Mesh &mesh) = delete;

    Mesh &operator=(const Mesh &mesh) = delete;

    MeshUniforms genMeshUniforms(const CstShaderProgram_sptr& shaderProgram) const;

    MeshGeometry genMeshGeometry() const;

private:

    const CstDisplayable_sptr _displayable;
    const vecCstGeometricShape_sptr _shapes;

    /**
     * Index where the geometry will fetch uniform data in vertex arrays.
     * Other meshes could have the same dynamic id.
     */
    const short _dynamicsId;
};

#endif // MESH_H
