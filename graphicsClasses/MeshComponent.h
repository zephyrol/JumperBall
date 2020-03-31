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
    MeshComponent ( const std::shared_ptr<const GeometricShape>& shape,
                    const std::shared_ptr<Animation>& anim);
    
    const std::shared_ptr<const GeometricShape>& shape() const ;
    const std::shared_ptr<Animation>&      animation() const; 
    ~MeshComponent();


private:
    
    const std::shared_ptr<const GeometricShape> _shape;
    const std::shared_ptr<Animation>            _animation; 

};

#endif /* MESHCOMPONENT_H */

