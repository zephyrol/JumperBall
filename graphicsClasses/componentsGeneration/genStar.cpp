/*
 * file: genStar.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "scene/mesh/MeshGenerator.h"

vecMesh_sptr MeshGenerator::genStar (const StarState& star) {
    GeometricShape_sptr shape = std::make_shared <Quad>(star.initialTransform());
    vecCstGeometricShape_sptr geometricShapes { shape };
    Mesh_sptr meshSptr = std::make_shared <Mesh>(star, geometricShapes);
    return { meshSptr };
}
