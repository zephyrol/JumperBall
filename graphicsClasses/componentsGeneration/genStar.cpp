/*
 * file: genStar.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "MeshGenerator.h"

vecMesh_sptr MeshGenerator::genStar (const Star& star) {
    CstGeometricShape_sptr shape = std::make_shared <Quad>();
    vecCstGeometricShape_sptr geometricShapes { shape };
    Frames_uptr <ObjectState> frame_ptr = Frames <ObjectState>::genFrames <Star, StarState>(star);
    Mesh_sptr meshSptr = std::make_shared <Mesh>(std::move(frame_ptr), std::move(geometricShapes));
    return { meshSptr };
}
