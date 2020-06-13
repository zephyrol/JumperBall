/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.cpp
 * Author: Morgenthaler S
 * 
 * Created on 28 avril 2020, 17:44
 */

#include <iostream>
#include <algorithm>
#include <math.h>
#include <cctype>
#include "Label.h"

Label::Label(float width, float height, const JumperBallTypes::vec2f& position,
             bool fixed):
    _width(width),
    _height(height),
    _children{},
    _position(position),
    _fixed(fixed)
{
}

float Label::width() const {
    return _width;
}

float Label::height() const {
    return _height;
}

const std::vector<std::shared_ptr<const Label> >& Label::children() const {
    return _children;
}

JumperBallTypes::vec2f Label::position() const {
    return _position;
}

std::string Label::message() const {
    return {};
}

bool Label::isFixed() const {
    return _fixed;
}


