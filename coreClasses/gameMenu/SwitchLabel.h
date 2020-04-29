#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H
#include "Label.h"

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


class SwitchLabel: Label
{
public:
    SwitchLabel(float width,float height,bool switchButton);

private:
    const bool _switch;
};

#endif // MESSAGELABEL_H
