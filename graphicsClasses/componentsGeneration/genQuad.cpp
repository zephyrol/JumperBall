/*
 * File:   genQuad.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genComponents(const GraphicQuad & ) {

    //The quad var is just used to specify that
    //we want to gen a MeshComponent from a Quad

    if (commonShapes.find("screenQuad") == commonShapes.end()) {
        commonShapes["screenQuad"] = std::make_shared<Quad> ();
    }

    MeshComponent_sptr component = std::make_shared<MeshComponent>(
            commonShapes.at("screenQuad"), nullptr);
    return vecMeshComponent_sptr {component};
}
