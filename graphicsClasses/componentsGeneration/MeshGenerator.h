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
#include <scene/items/Item.h>
#include <scene/enemies/Enemy.h>
#include <scene/special/Special.h>
#include <scene/Star.h>
#include <gameMenu/Page.h>
#include "geometry/Cylinder.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "geometry/Triangle.h"
#include "system/Geometry.h"
#include "FrameBuffer.h"
#include <scene/Mesh.h>
#include "graphicMenu/FontTexturesGenerator.h"


namespace MeshGenerator {

vecMesh_sptr genBall(const std::shared_ptr<const Ball>& ball);
vecMesh_sptr genBlocks(const CstMap_sptr& map);
vecMesh_sptr genItems(const CstMap_sptr& map);
vecMesh_sptr genEnemies(const CstMap_sptr& map);
vecMesh_sptr genSpecials(const CstMap_sptr& map);
vecMesh_sptr genStar(const std::shared_ptr<const Star>& star);
vecMesh_sptr genScreen();


Mesh_sptr genLetterLabel(
    const CstPage_sptr& page,
    const JBTypes::vec2f& position,
    float width,
    float height,
    const glm::vec3& color
);

std::map <unsigned char, vecMesh_sptr> genLettersLabel(
    const CstPage_sptr& page,
    const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet
    );
vecMesh_sptr genOthersLabel(const CstPage_sptr& page);

    Mesh_sptr genBlock(const CstMap_sptr &map, const CstBlock_sptr &block);

    vecCstGeometricShape_sptr genSharps(const Block& block,
                                    const Map::BlockTypes& type,
                                    const glm::vec3& posWorld);

vecCstGeometricShape_sptr genJumpers(const Block& block,
                                     const Map::BlockTypes& type,
                                     const glm::vec3& posWorld);

Mesh_sptr genItem(const std::shared_ptr<const Item>& item);
Mesh_sptr genEnemy(const std::shared_ptr<const Enemy>& enemy, const Map::EnemyTypes& category);
Mesh_sptr genSpecial(const std::shared_ptr<const Special>& special, const Map::SpecialTypes& category);

vecCstGeometricShape_sptr genGeometricShapesFromLabel(const Label& label);

}

#endif /* MESHGENERATOR_H */
