//
// Created by S.Morgenthaler on 23/08/2023.
//

#include "StarGroupGenerator.h"
#include "geometry/Quad.h"

StarGroupGenerator::StarGroupGenerator(std::shared_ptr<const Star> star, std::shared_ptr<const Star> star2) :
    _star(std::move(star)),
    _star2(std::move(star2)) {
}

RenderGroup_sptr StarGroupGenerator::genRenderGroup() const {
    CstGeometricShape_sptr shape = std::make_shared<Quad>();
    vecCstGeometricShape_sptr geometricShapes{shape};
    Mesh_sptr meshStar = std::make_shared<Mesh>(_star, std::move(geometricShapes), 0);

    CstGeometricShape_sptr shape2 = std::make_shared<Quad>();
    vecCstGeometricShape_sptr geometricShapes2{shape2};
    Mesh_sptr meshStar2 = std::make_shared<Mesh>(_star2, std::move(geometricShapes2), 1);

    auto meshDynamicGroup = std::unique_ptr<MeshDynamicGroup>(new MeshDynamicGroup(
        {{meshStar},
         {meshStar2}}
    ));
    return RenderGroup::createInstance(std::move(meshDynamicGroup));
}
