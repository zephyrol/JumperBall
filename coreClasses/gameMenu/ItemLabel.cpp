/*
 * File: ItemLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#include "ItemLabel.h"

ItemLabel::ItemLabel(
    const Label::WidthUnit &widthUnit,
    const std::function<Label::LabelDimensions(float)> &updateLabelSizesFct,
    float ratio
) :
    Label(widthUnit, updateLabelSizesFct, ratio) {
}
