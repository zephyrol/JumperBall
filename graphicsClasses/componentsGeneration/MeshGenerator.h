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

    CstGeometricShape_sptr createGeometricShape(const CstShape_sptr &shape);

    vecMesh_sptr genBall(const CstBall_sptr &ball);

    vecMesh_sptr genBlocks(const CstMap_sptr &map);

    vecMesh_sptr genItems(const CstMap_sptr &map);

    vecMesh_sptr genEnemies(const CstMap_sptr &map);

    vecMesh_sptr genSpecials(const CstMap_sptr &map);

    vecMesh_sptr genStars(const std::shared_ptr<const Star> &star, const std::shared_ptr<const Star> &star2);

    vecMesh_sptr genScreen();

    Mesh_sptr genBlock(const CstMap_sptr &map, const CstBlock_sptr &block);

    Mesh_sptr genItem(const std::shared_ptr<const Item> &item);

    vecCstGeometricShape_sptr genGeometricShapesFromLabel(const Label &label);

    template<typename T>
    vecMesh_sptr genMeshes(const T& shapesGenerator);

}

template<typename T>
vecMesh_sptr MeshGenerator::genMeshes(const T& shapesGenerator) {
    vecMesh_sptr meshes;
    vecCstGeometricShape_sptr geometricShapes;
    for(const auto& shape: shapesGenerator->getShapes()) {
        geometricShapes.push_back(createGeometricShape(shape));
    }
    meshes.push_back(std::make_shared <Mesh>(shapesGenerator, std::move(geometricShapes)));
    return meshes;
}

#endif /* MESHGENERATOR_H */
