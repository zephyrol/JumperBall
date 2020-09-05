/*
 * file:   genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "../MeshGenerator.h"

VecMeshComponentSptr MeshGenerator::genComponents(const Star& star) {

    if (commonShapes.find("starQuad") == commonShapes.end()) {
        commonShapes["starQuad"] = std::make_shared<Quad> ();
    }
    const MeshComponentSptr component = std::make_shared<MeshComponent>
            (std::make_shared<Quad>( *commonShapes.at("starQuad"),
                                     star.initialTransform()),
             nullptr);
    return VecMeshComponentSptr {component};
}

