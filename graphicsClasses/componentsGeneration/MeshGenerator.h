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
#include <scene/MapState.h>
#include <scene/objects/ObjectState.h>
#include <scene/enemies/EnemyState.h>
#include <scene/StarState.h>
#include <gameMenu/Page.h>
#include "geometry/Cylinder.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "geometry/QuadState.h"
#include "FrameBuffer.h"
#include <scene/Mesh.h>
#include "graphicMenu/FontTexturesGenerator.h"


namespace MeshGenerator {

// vecMeshComponent_sptr genComponents(const QuadState& quad);
vecMesh_sptr genBall(const Ball& ball);
vecMesh_sptr genBlocks(const Map& map);
vecMesh_sptr genObjects(const Map& map);
vecMesh_sptr genEnemies(const Map& map);
vecMesh_sptr genStar(const Star& star);
vecMesh_sptr genQuad(const Quad& quad);


Mesh_sptr genQuadLabel(const JBTypes::vec2f& position, float width, float height, const glm::vec3& color);
std::map <unsigned char, vecMesh_sptr> genLettersLabel(
    const Page& page,
    const FontTexturesGenerator::GraphicAlphabet& graphicAlphabet
    );

// vecMeshComponent_sptr genObject(const ObjectState& obj, const glm::vec3& position, const JBTypes::Dir& dir);

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
/*vecMeshComponent_sptr genEnemy(const EnemyState& enemy);
   vecMeshComponent_sptr genSpecial(const SpecialState& special);

   vecMeshComponent_sptr sortComponents( const vecMeshComponent_sptr& components);*/
// extern std::map <std::string, GeometricShape_sptr> commonShapes;
}

#endif /* MESHGENERATOR_H */
