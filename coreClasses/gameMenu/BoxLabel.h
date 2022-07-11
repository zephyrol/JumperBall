/*
 * File: ItemLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 13 juin 2020
 */

#ifndef BOXLABEL_H
#define BOXLABEL_H

#include "Label.h"

class BoxLabel : public Label {
public:
    BoxLabel(
        const Label::WidthUnit &widthUnit,
        const std::function<Label::LabelDimensions(float)> &updateLabelSizesFct,
        float ratio
    );

    virtual ~BoxLabel() = default;

private:
};

#endif // BOXLABEL_H
