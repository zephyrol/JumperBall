/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ObjectLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#ifndef OBJECTLABEL_H
#define OBJECTLABEL_H
#include "Label.h"


class ObjectLabel: public Label
{
public:
    ObjectLabel(
        const Label::WidthUnit& widthUnit,
        float width,
        float height,
        const JBTypes::vec2f& position,
        const Object::CategoryOfObjects& categoryOfObject
    );

private:
    const Object::CategoryOfObjects _categoryOfObjects;
};

#endif // OBJECTLABEL_H
