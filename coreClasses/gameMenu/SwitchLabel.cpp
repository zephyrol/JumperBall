/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
*/

/*
 * File:   SwitchLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 avril 2020, 19:43
*/

#include "SwitchLabel.h"

SwitchLabel::SwitchLabel(
    const Label::WidthUnit &widthUnit,
    float width,
    float height,
    const JBTypes::vec2f &position,
    bool switchButton):
    Label(widthUnit, width, height, position),
    _switch(switchButton)
    {}

void SwitchLabel::turnOn()
{
    _switch = true;
}
void SwitchLabel::turnOff()
{
    _switch = false;
}
