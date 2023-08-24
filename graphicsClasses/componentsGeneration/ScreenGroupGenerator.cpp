//
// Created by S.Morgenthaler on 23/08/2023.
//

#include "ScreenGroupGenerator.h"
#include "scene/Screen.h"
#include "geometry/Quad.h"

RenderGroup_sptr ScreenGroupGenerator::genRenderGroup() const {
    CstGeometricShape_sptr shape = std::make_shared<Quad>();
    vecCstGeometricShape_sptr geometricShapes{shape};
    Mesh_sptr meshScreen = std::make_shared<Mesh>(std::make_shared<Screen>(), std::move(geometricShapes), 0);

    auto meshDynamicGroup = std::unique_ptr<MeshDynamicGroup>(new MeshDynamicGroup(
        {{meshScreen}}
    ));
    return RenderGroup::createInstance(std::move(meshDynamicGroup));
}
