/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ObjectLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#include "ObjectLabel.h"

ObjectLabel::ObjectLabel(float width, float height, 
    const std::shared_ptr<Object>& object): 
    Label(width,height),
    _object(object)
{ }

Label::TypeOfLabel ObjectLabel::typeOfLabel() const {
    return Label::TypeOfLabel::Object;
 }
