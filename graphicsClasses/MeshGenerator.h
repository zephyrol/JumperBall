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
#include "MeshComponent.h"
#include "Utility.h"
#include "Star.h"
#include <objects/Object.h>
#include "geometry/Cylinder.h"
#include "geometry/Pyramid.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Quad.h"
#include "animations/BallAnimation.h"
#include "animations/ObjectAnimation.h"
#include "animations/BlockAnimation.h"
#include "FrameBuffer.h"
#include "MeshComponent.h"


namespace MeshGenerator {

    vecMeshComponent_sptr  genComponents(const Quad& quad);
    vecMeshComponent_sptr  genComponents(const Ball& ball);
    vecMeshComponent_sptr  genComponents(const Map& map);
    vecMeshComponent_sptr  genComponents(const Star& star);
    vecMeshComponent_sptr  sortComponents(vecMeshComponent_sptr& components);

    vecMeshComponent_sptr  genObject(const std::shared_ptr<const Object>& obj,
                                    const glm::vec3& position,
                                    const JBTypes::Dir& dir);
    
    vecMeshComponent_sptr  genBlock(const Map& map, size_t index);
    vecMeshComponent_sptr  genSharps(const Block& block,
                                    const Map::BlockTypes& type,
                                    const glm::vec3& posWorld);
    vecMeshComponent_sptr  genJumpers(const Block& block,
                                     const Map::BlockTypes& type,
                                     const glm::vec3& posWorld);

    extern std::map<std::string, std::shared_ptr<GeometricShape> >
                                commonShapes;
}

#endif /* MESHGENERATOR_H */

