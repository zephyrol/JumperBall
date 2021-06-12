/*
 * File: ItemLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 13 juin 2020
 */

#ifndef BOXLABEL_H
#define BOXLABEL_H
#include "Label.h"
class BoxLabel:public Label {
public:
BoxLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position);
virtual ~BoxLabel() = default;

private:
};

#endif // BOXLABEL_H
