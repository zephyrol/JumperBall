/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeshComponent.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 31 mars 2020, 00:35
 */

#include "MeshComponent.h"

MeshComponent::MeshComponent(const std::shared_ptr<const GeometricShape>& elem, 
                             const std::shared_ptr<Animation>& anim):
    _shape(elem),
    _animation(anim),
    _cstAnimation(std::const_pointer_cast<const Animation>(_animation))
{

}

const std::shared_ptr<const GeometricShape>& MeshComponent::shape() const {
    return _shape;
}

const std::shared_ptr<Animation>& MeshComponent::animation() {
    return _animation;
}

const std::shared_ptr<const Animation>& MeshComponent::animation() const{
    return _cstAnimation;
}
