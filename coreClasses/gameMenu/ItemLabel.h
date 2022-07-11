/*
 * File: ItemLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#ifndef OBJECTLABEL_H
#define OBJECTLABEL_H

#include "Label.h"


class ItemLabel : public Label {
public:
    ItemLabel(
        const Label::WidthUnit &widthUnit,
        const std::function<Label::LabelDimensions(float)> &updateLabelSizesFct,
        float ratio
    );

};

#endif // OBJECTLABEL_H
