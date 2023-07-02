/*
 * File: MeshGenerator.h
 * Author: Morgenthaler S
 *
 * Created on 12 avril 2020, 19:24
 */

#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#include "Utility.h"
#include <scene/Ball.h>
#include <scene/blocks/Block.h>
#include <scene/blocks/items/Item.h>
#include <scene/blocks/enemies/Enemy.h>
#include <scene/blocks/special/Special.h>
#include <scene/Star.h>
#include "gameMenu/pages/Page.h"
#include "geometry/Cylinder.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "geometry/Triangle.h"
#include "gameMenu/LabelGeometry.h"
#include "frameBuffer/FrameBuffer.h"
#include "process/mesh/Mesh.h"
#include "graphicMenu/FontTexturesGenerator.h"
#include "process/mesh/MeshDynamicGroup.h"


namespace MeshGenerator {

    CstMesh_sptr genMesh(
        const CstDisplayable_sptr &displayable,
        short dynamicsId
    );

    CstGeometricShape_sptr createGeometricShape(const CstShape_sptr &shape);

    MeshDynamicGroup_uptr genBall(const CstBall_sptr &ball, unsigned int ballSkin);

    MeshDynamicGroup_uptr genBlocks(const CstMap_sptr &map);

    MeshDynamicGroup_uptr genItems(const CstMap_sptr &map);

    MeshDynamicGroup_uptr genEnemies(const CstMap_sptr &map);

    MeshDynamicGroup_uptr genSpecials(const CstMap_sptr &map);

    MeshDynamicGroup_uptr genStars(const std::shared_ptr<const Star> &star, const std::shared_ptr<const Star> &star2);

    MeshDynamicGroup_uptr genScreen();

    MeshDynamicGroup_uptr genMeshDynamicGroup(
        const vecCstDisplayable_sptr &displayableVector,
        std::unique_ptr<std::function<CstMesh_sptr(
            const CstDisplayable_sptr &, short
        )>> customMeshGenerationFunction = nullptr
    );

    CstMesh_sptr genBlock(const CstMap_sptr &map, const CstBlock_sptr &block, short dynamicsId);

    vecCstGeometricShape_sptr genGeometricShapesFromLabel(const Label &label);

}

#endif /* MESHGENERATOR_H */
