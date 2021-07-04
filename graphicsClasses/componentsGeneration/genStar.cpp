/*
 * file: genStar.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "MeshGenerator.h"

vecMesh_sptr MeshGenerator::genStar (const std::shared_ptr<const Star>& star) {
    CstGeometricShape_sptr shape = std::make_shared <Quad>();
    vecCstGeometricShape_sptr geometricShapes { shape };
    Mesh_sptr meshSptr = std::make_shared <Mesh>(star, std::move(geometricShapes));
    return { meshSptr };
}
