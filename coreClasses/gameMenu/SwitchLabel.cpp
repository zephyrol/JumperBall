#include "SwitchLabel.h"
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SwitchLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

SwitchLabel::SwitchLabel(float width, float height, 
    const JumperBallTypes::vec2f& position,
    bool switchButton):
    Label(width,height,position),
    _switch(switchButton)
{ }

Label::TypeOfLabel SwitchLabel::typeOfLabel() const {
    return Label::TypeOfLabel::Switch;
}

void SwitchLabel::turnOn() { 
    _switch = true;
}

void SwitchLabel::turnOff() {
    _switch = false;

}

