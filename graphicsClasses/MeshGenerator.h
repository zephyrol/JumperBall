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

    VecMeshComponentSptr  genComponents(const Quad& quad);
    VecMeshComponentSptr  genComponents(const Ball& ball);
    VecMeshComponentSptr  genComponents(const Map& map);
    VecMeshComponentSptr  genComponents(const Star& star);
    VecMeshComponentSptr  sortComponents(VecMeshComponentSptr& components);

    VecMeshComponentSptr  genObject(const std::shared_ptr<const Object>& obj,
                                    const glm::vec3& position,
                                    const JBTypes::Dir& dir);
    
    VecMeshComponentSptr  genBlock(const Map& map, size_t index);
    VecMeshComponentSptr  genSharps(const Block& block,
                                    const Map::BlockTypes& type,
                                    const glm::vec3& posWorld);
    VecMeshComponentSptr  genJumpers(const Block& block,
                                     const Map::BlockTypes& type,
                                     const glm::vec3& posWorld);

    extern std::map<std::string, std::shared_ptr<GeometricShape> >
                                commonShapes;
}

#endif /* MESHGENERATOR_H */

