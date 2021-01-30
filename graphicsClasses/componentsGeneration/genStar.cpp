/*
 * file: genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genComponents (const StarState& star) {

    if (commonShapes.find("starQuad") == commonShapes.end()) {
        commonShapes["starQuad"] = std::make_shared <Quad>();
    }
    const MeshComponent_sptr component = std::make_shared <MeshComponent>
                                             (std::make_shared <Quad>(*commonShapes.at("starQuad"),
                                                                      star.initialTransform()),
                                             nullptr);
    return vecMeshComponent_sptr { component };
}
