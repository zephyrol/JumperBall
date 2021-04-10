/*
 * File: genQuad.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"

vecMesh_sptr MeshGenerator::genQuad (const Quad& quad) {
    CstGeometricShape_sptr shape = std::make_shared <Quad>();
    vecCstGeometricShape_sptr geometricShapes { shape };
    State_uptr state_ptr(new QuadState(quad));
    Mesh_sptr meshSptr = std::make_shared <Mesh>(std::move(state_ptr), std::move(geometricShapes));
    return { meshSptr };
}
