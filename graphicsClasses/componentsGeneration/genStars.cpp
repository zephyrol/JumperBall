/*
 * file: genStar.cpp
 * author: morgenthaler s
 *
 * created on 22 aout 2020, 18h15
 */

#include "MeshGenerator.h"

MeshDynamicGroup_uptr MeshGenerator::genStars(
    const std::shared_ptr<const Star> &star,
    const std::shared_ptr<const Star> &star2
) {
    CstGeometricShape_sptr shape = std::make_shared<Quad>();
    vecCstGeometricShape_sptr geometricShapes{shape};
    Mesh_sptr meshStar = std::make_shared<Mesh>(star, std::move(geometricShapes), 0);

    CstGeometricShape_sptr shape2 = std::make_shared<Quad>();
    vecCstGeometricShape_sptr geometricShapes2{shape2};
    Mesh_sptr meshStar2 = std::make_shared<Mesh>(star2, std::move(geometricShapes2), 1);

    return std::unique_ptr<MeshDynamicGroup>(new MeshDynamicGroup(
        {{meshStar}, {meshStar2}}
    ));
}
