/*
 * File: MeshGenerator.h
 * Author: Morgenthaler S
 *
 * Created on 12 avril 2020, 19:24
 */

#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H
#include "Utility.h"
#include <scene/BallState.h>
#include <scene/blocks/BlockState.h>
#include <scene/objects/ObjectState.h>
#include <scene/enemies/EnemyState.h>
#include <scene/special/SpecialState.h>
#include <scene/StarState.h>
#include <gameMenu/Page.h>
#include "geometry/Cylinder.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "geometry/Triangle.h"
#include "geometry/QuadState.h"
#include "system/Geometry.h"
#include "FrameBuffer.h"
#include <scene/Mesh.h>
#include "graphicMenu/FontTexturesGenerator.h"


namespace MeshGenerator {

vecMesh_sptr genBall(const Ball& ball);
vecMesh_sptr genBlocks(const Map& map);
vecMesh_sptr genObjects(const Map& map);
vecMesh_sptr genEnemies(const Map& map);
vecMesh_sptr genSpecials(const Map& map);
vecMesh_sptr genStar(const Star& star);
vecMesh_sptr genQuad(const Quad& quad);


Mesh_sptr genQuadLabel(const JBTypes::vec2f& position, float width, float height, const glm::vec3& color);
std::map <unsigned char, vecMesh_sptr> genLettersLabel(
    const Page& page,
    const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet
    );
vecMesh_sptr genOthersLabel(const Page& page);

Mesh_sptr genBlock(const Map& map, size_t index);

vecCstGeometricShape_sptr genSharps(const Block& block,
                                    const Map::BlockTypes& type,
                                    const glm::vec3& posWorld);

vecCstGeometricShape_sptr genJumpers(const Block& block,
                                     const Map::BlockTypes& type,
                                     const glm::vec3& posWorld);

Mesh_sptr genObject(const Object& object);
Mesh_sptr genEnemy(const Enemy& enemy, const Map::EnemyTypes& category);
Mesh_sptr genSpecial(const Special& special, const Map::SpecialTypes& category);

vecCstGeometricShape_sptr genGeometricShapesFromLabel(const Label& label);
}

#endif /* MESHGENERATOR_H */
