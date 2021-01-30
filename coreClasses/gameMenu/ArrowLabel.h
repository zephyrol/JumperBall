/*
 * File: ArrowLabel.h
 * Author: Morgenthaler S
 *
 * Created on 16 janvier 2021, 21:42
 */

#ifndef ARROWLABEL_H
#define ARROWLABEL_H
#include "Label.h"

class ArrowLabel:public Label {
public:
ArrowLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    const std::shared_ptr <LabelAnswer> action = nullptr,
    bool activated = true
    );
virtual ~ArrowLabel() = default;

private:
};

#endif // ARROWLABEL_H
