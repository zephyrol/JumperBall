/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Map.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H
#include "Label.h"


class ObjectLabel: Label
{
public:
    ObjectLabel(float width,float height,
                const std::shared_ptr<Object>& object);

private:
    const std::shared_ptr<Object>_object;
};

#endif // MESSAGELABEL_H
