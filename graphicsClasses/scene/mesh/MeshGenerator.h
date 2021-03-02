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
#include "geometry/Cylinder.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "geometry/QuadState.h"
#include "StarState.h"
#include "FrameBuffer.h"
#include <scene/Mesh.h>


namespace MeshGenerator {

// vecMeshComponent_sptr genComponents(const QuadState& quad);
// vecMeshComponent_sptr genComponents(const BallState& ball);
vecMesh_sptr genMeshes(const Map& map);
vecMesh_sptr genMeshes(const Star& star);

// vecMeshComponent_sptr genObject(const ObjectState& obj, const glm::vec3& position, const JBTypes::Dir& dir);

Mesh_sptr genBlock(const Map& map, size_t index);

vecCstGeometricShape_sptr genSharps(const Block& block,
                                    const Map::BlockTypes& type,
                                    const glm::vec3& posWorld);

vecCstGeometricShape_sptr genJumpers(const Block& block,
                                     const Map::BlockTypes& type,
                                     const glm::vec3& posWorld);
/*vecMeshComponent_sptr genEnemy(const EnemyState& enemy);
   vecMeshComponent_sptr genSpecial(const SpecialState& special);

   vecMeshComponent_sptr sortComponents( const vecMeshComponent_sptr& components);*/
extern std::map <std::string, GeometricShape_sptr> commonShapes;
}

#endif /* MESHGENERATOR_H */
