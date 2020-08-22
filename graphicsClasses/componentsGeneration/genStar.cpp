/*
 * file:   genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "../MeshGenerator.h"

std::vector<MeshComponent> MeshGenerator::genComponents(const Star& star) {

    if (commonShapes.find("starQuad") == commonShapes.end()) {
        commonShapes["starQuad"] = std::make_shared<Quad> ();
    }
    const MeshComponent component ( std::make_shared<Quad>(
                                    *commonShapes.at("starQuad"),
                                    star.initialTransform()),
                                nullptr);
    return std::vector<MeshComponent> {component};
}

