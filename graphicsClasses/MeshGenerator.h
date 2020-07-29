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
#include "FrameBuffer.h"
#include "MeshComponent.h"


namespace MeshGenerator {

    std::vector<MeshComponent>  genComponents(const Quad& quad);
    std::vector<MeshComponent>  genComponents(const Ball& ball);
    std::vector<MeshComponent>  genComponents(const Map& map);
    std::vector<MeshComponent>  genComponents(const Star& star);
    std::vector<MeshComponent>  genComponents(
                                    const std::shared_ptr<const Object>& obj,
                                    const glm::vec3& position,
                                    const JBTypes::Dir& dir);


    std::vector<MeshComponent>  sortComponents(std::vector<MeshComponent>&
                                                  components);
    
    std::vector<MeshComponent>  blockManager( const Block& block, 
                                              const std::array<unsigned int,3>& 
                                                            position);
    std::vector<MeshComponent>  genSharps(  const Block& block, 
                                            const glm::vec3& posWorld);
    std::vector<MeshComponent>  genJumpers( const Block& block, 
                                            const glm::vec3& posWorld);

    extern std::map<std::string, std::shared_ptr<GeometricShape> >
                                commonShapes;
}

#endif /* MESHGENERATOR_H */

