/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File: SwitchLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 avril 2020, 19:43
 */

#include "SwitchLabel.h"

SwitchLabel::SwitchLabel(
    const Label::WidthUnit& widthUnit,
    const std::function<Label::LabelDimensions(float)>& updateLabelSizesFct,
    float ratio,
    bool switchButton):
    Label(widthUnit, updateLabelSizesFct, ratio),
    _switch(switchButton) {
}

void SwitchLabel::turnOn() {
    _switch = true;
}
void SwitchLabel::turnOff() {
    _switch = false;
}
