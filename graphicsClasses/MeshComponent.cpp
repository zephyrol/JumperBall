/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeshComponent.cpp
 * Author: seb
 * 
 * Created on 31 mars 2020, 00:35
 */

#include "MeshComponent.h"

MeshComponent::MeshComponent(const std::shared_ptr<GeometricShape>& elem, 
                             const std::shared_ptr<Animation>& anim):
                      _shape(elem),
                      _animation(anim){

}

const std::shared_ptr<GeometricShape>& MeshComponent::shape() const {
    return _shape;
}

const std::shared_ptr<Animation>& MeshComponent::animation() const {
    return _animation;
}




