/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Animation.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 janvier 2020, 11:19
 */

#include "Animation.h"

Animation::Animation():
    _model(1.f),
    _translation(1.f),
    _scaleRotation(1.f)
{}

glm::mat4 Animation::model() const
{
    return _model;
}

glm::mat4 Animation::scaleRotation() const
{
    return _scaleRotation;
}

glm::mat4 Animation::translation() const
{
    return _translation;
}
