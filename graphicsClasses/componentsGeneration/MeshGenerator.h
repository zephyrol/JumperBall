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


namespace MeshGenerator {

    /**
     * Each vecMesh_sptr in this vector contains the same dynamicId. So only the first mesh of each list can be used
     * to generate the uniforms and to update them.
     */
    struct MeshDynamicGroup {
        std::vector<vecMesh_sptr> staticMeshes;
        std::vector<vecMesh_sptr> dynamicMeshes;
    };

    Mesh_sptr genMesh(
        const CstDisplayable_sptr &displayable,
        short dynamicsId
    );

    CstGeometricShape_sptr createGeometricShape(const CstShape_sptr &shape);

    MeshDynamicGroup genBall(const CstBall_sptr &ball, unsigned int ballSkin);

    MeshDynamicGroup genBlocks(const CstMap_sptr &map);

    MeshDynamicGroup genItems(const CstMap_sptr &map);

    MeshDynamicGroup genEnemies(const CstMap_sptr &map);

    MeshDynamicGroup genSpecials(const CstMap_sptr &map);

    MeshDynamicGroup genStars(const std::shared_ptr<const Star> &star, const std::shared_ptr<const Star> &star2);

    MeshDynamicGroup genScreen();

    MeshDynamicGroup genMeshDynamicGroup(
        const vecCstDisplayable_sptr &displayableVector,
        std::unique_ptr<std::function<Mesh_sptr(
            const CstDisplayable_sptr &, short
        )>> customMeshGenerationFunction = nullptr
    );

    Mesh_sptr genBlock(const CstMap_sptr &map, const CstBlock_sptr &block, short dynamicsId);

    Mesh_sptr genItem(const std::shared_ptr<const Item> &item);

    vecCstGeometricShape_sptr genGeometricShapesFromLabel(const Label &label);

}

#endif /* MESHGENERATOR_H */
