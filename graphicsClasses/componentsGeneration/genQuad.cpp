/*
 * File:   genQuad.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "../MeshGenerator.h"

VecMeshComponentSptr MeshGenerator::genComponents(const Quad& ) {

    //The quad var is just used to specify that
    //we want to gen a MeshComponent from a Quad

    if (commonShapes.find("screenQuad") == commonShapes.end()) {
        commonShapes["screenQuad"] = std::make_shared<Quad> ();
    }

    MeshComponentSptr component = std::make_shared<MeshComponent>(
            commonShapes.at("screenQuad"), nullptr);
    return VecMeshComponentSptr {component};
}
