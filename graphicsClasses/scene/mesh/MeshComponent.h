/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeshComponent.h
 * Author: seb
 *
 * Created on 31 mars 2020, 00:35
 */

#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H
#include "geometry/GeometricShape.h"
#include "animations/Animation.h"

class MeshComponent {
public:
    //--CONSTRUCTORS & DESTRUCTORS--//
    MeshComponent ( const std::shared_ptr<const GeometricShape>& shape,
                    const std::shared_ptr<Animation>& anim);


    //-------CONST METHODS----------//
    const std::shared_ptr<const GeometricShape>&  shape()                 const;
    const std::shared_ptr<const Animation>&       animation()             const;

    //----------METHODS-------------//
    const std::shared_ptr<Animation>&             animation();


private:
    
    //--------ATTRIBUTES-----------//
    const std::shared_ptr<const GeometricShape>   _shape;
    const std::shared_ptr<Animation>              _animation; 
    const std::shared_ptr<const Animation>        _cstAnimation;
};

//------------TYPES------------//
using MeshComponent_sptr = std::shared_ptr<MeshComponent>;
using CstMeshComponent_sptr = std::shared_ptr<const MeshComponent>;
using vecMeshComponent_sptr = std::vector<MeshComponent_sptr>;
using vecCstMeshComponent_sptr = std::vector<CstMeshComponent_sptr>;

#endif /* MESHCOMPONENT_H */
