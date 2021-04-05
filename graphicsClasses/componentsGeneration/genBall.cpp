/*
 * file: genball.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "scene/MeshGenerator.h"

vecMesh_sptr MeshGenerator::genBall (const Ball& ball) {

    const CstGeometricShape_sptr sphere = std::make_shared <const Sphere>();
    vecCstGeometricShape_sptr geometricShapes { sphere };
    Mesh_sptr sphereMesh = std::make_shared <Mesh>(
        std::unique_ptr <State>(new BallState(ball)),
        std::move(geometricShapes)
        );
    return { sphereMesh };
}
