/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ArrowLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */
#include "ArrowLabel.h"

ArrowLabel::ArrowLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    const std::shared_ptr<LabelAnswer> action,
    bool activated):
    Label(widthUnit, width, height, position, activated, action)
{
}
