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

Label::Label(float width, float height):
    _width(width),
    _height(height),
    _children{}
{
}

float Label::width() const
{
    return _width;
}

float Label::height() const
{
    return _height;
}

std::vector<std::shared_ptr<Label::Child> > Label::children() const
{
    return _children;
}
