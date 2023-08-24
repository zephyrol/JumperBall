/*
 * File: MeshGenerator.h
 * Author: Morgenthaler S
 *
 * Created on 12 avril 2020, 19:24
 */

#ifndef RENDER_GROUP_GENERATOR_H
#define RENDER_GROUP_GENERATOR_H

#include "process/RenderGroup.h"

class RenderGroupGenerator {

public:
    virtual RenderGroup_sptr genRenderGroup() const = 0;
};

//namespace MeshGenerator {
//
//    CstMesh_sptr genMesh(
//        const CstDisplayable_sptr &displayable,
//        short dynamicsId
//    );
//
//    CstGeometricShape_sptr createGeometricShape(const CstShape_sptr &shape);
//
//    MeshDynamicGroup_uptr genBall(const CstBall_sptr &ball, unsigned int ballSkin);
//
//    u
//        MeshDynamicGroup_uptr
//
//    genBlocks(const CstMap_sptr &map);
//
//    MeshDynamicGroup_uptr genItems(const CstMap_sptr &map);
//
//    MeshDynamicGroup_uptr genEnemies(const CstMap_sptr &map);
//
//    MeshDynamicGroup_uptr genSpecials(const CstMap_sptr &map);
//
//    MeshDynamicGroup_uptr genStars(const std::shared_ptr<const Star> &star, const std::shared_ptr<const Star> &star2);
//
//    MeshDynamicGroup_uptr genScreen();
//
//    MeshDynamicGroup_uptr genMeshDynamicGroup(
//        const vecCstDisplayable_sptr &displayableVector,
//        std::unique_ptr<std::function<CstMesh_sptr(
//            const CstDisplayable_sptr &, short
//        )>> customMeshGenerationFunction = nullptr
//    );
//
//    CstMesh_sptr genBlock(const CstMap_sptr &map, const CstBlock_sptr &block, short dynamicsId);
//
//    vecCstGeometricShape_sptr genGeometricShapesFromLabel(const Label &label);
//
//}

#endif
