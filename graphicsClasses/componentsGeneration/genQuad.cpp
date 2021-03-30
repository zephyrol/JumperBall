/*
 * File: genQuad.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"

vecMesh_sptr MeshGenerator::genQuad (const Quad& quad) {
    CstGeometricShape_sptr shape = std::make_shared <Quad>();
    vecCstGeometricShape_sptr geometricShapes { shape };
    std::unique_ptr <State> state_ptr(new QuadState(quad));
    Mesh_sptr meshSptr = std::make_shared <Mesh>(std::move(state_ptr), std::move(geometricShapes));
    return { meshSptr };
}
/*vecMeshComponent_sptr MeshGenerator::genComponents (const QuadState&) {

    // The quad var is just used to specify that
    // we want to gen a MeshComponent from a Quad

    if (commonShapes.find("screenQuad") == commonShapes.end()) {
        commonShapes["screenQuad"] = std::make_shared <Quad>();
    }

    MeshComponent_sptr component = std::make_shared <MeshComponent>(
        commonShapes.at("screenQuad"), nullptr);
    return vecMeshComponent_sptr { std::move(component) };
   }
 */