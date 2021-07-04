/*
 * File: genScreen.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 juillet 2021, 8h50
 */
#include "MeshGenerator.h"
#include "scene/Screen.h"

vecMesh_sptr MeshGenerator::genScreen () {
    CstGeometricShape_sptr shape = std::make_shared <Quad>();
    vecCstGeometricShape_sptr geometricShapes { shape };
    Mesh_sptr meshSptr = std::make_shared <Mesh>(
        std::make_shared<Screen>(),
        std::move(geometricShapes));
    return { meshSptr };
}
