
/*
 * File:   BoxLabel.h
 * Author: Morgenthaler S
 *
 * Created on 13 juin 2020
 */

#include "BoxLabel.h"

BoxLabel::BoxLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position):
    Label(widthUnit, width, height, position)
{}
