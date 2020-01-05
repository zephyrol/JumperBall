/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Animation.cpp
 * Author: seb
 * 
 * Created on 5 janvier 2020, 11:19
 */

#include "Animation.h"

Animation::Animation(): 
            _scale(1.f),
            _rotation(1.f),
            _translation(1.f) 
{
}

glm::mat4 Animation::model() const {
    return translation() * scaleRotation();
}

glm::mat4 Animation::scaleRotation() const {
    //For our animations, we apply the scale after the rotation
    return _scale * _rotation;
}

glm::mat4 Animation::translation() const {
    return _translation;
}


Animation::~Animation() {

}

