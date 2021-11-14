/*
 * File: BoxLabel.h
 * Author: Morgenthaler S
 *
 * Created on 13 juin 2020
 */

#include <functional>
#include "BoxLabel.h"

BoxLabel::BoxLabel(
    const Label::WidthUnit& widthUnit,
    const std::function<Label::LabelDimensions(float)>& updateLabelSizesFct,
    float ratio
):
    Label(widthUnit, updateLabelSizesFct, ratio) {
}
