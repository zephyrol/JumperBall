/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeshGenerator.h
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
#include "MeshComponent.h"


namespace MeshGenerator {

    vecMeshComponent_sptr  genComponents(const QuadState& quad);
    vecMeshComponent_sptr  genComponents(const BallState& ball);
    vecMeshComponent_sptr  genComponents(const MapState& map);
    vecMeshComponent_sptr  genComponents(const StarState& star);

    vecMeshComponent_sptr  genObject(const ObjectState& obj,
                                    const glm::vec3& position,
                                    const JBTypes::Dir& dir);

    vecMeshComponent_sptr  genBlock(const MapState &map, size_t index);
    vecMeshComponent_sptr  genSharps(const BlockState& BlockState,
                                    const Map::BlockTypes& type,
                                    const glm::vec3& posWorld);
    vecMeshComponent_sptr  genJumpers(const BlockState& BlockState,
                                     const Map::BlockTypes& type,
                                     const glm::vec3& posWorld);
    vecMeshComponent_sptr  genEnemy(const EnemyState& enemy);
    vecMeshComponent_sptr  genSpecial(const SpecialState& special);

    vecMeshComponent_sptr  sortComponents(
                                const vecMeshComponent_sptr& components);
    extern std::map<std::string, std::shared_ptr<GeometricShape> >
                                commonShapes;
}

#endif /* MESHGENERATOR_H */

