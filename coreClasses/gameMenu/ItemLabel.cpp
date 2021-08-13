/*
 * File: ItemLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#include "ItemLabel.h"

ItemLabel::ItemLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position
    ):
    Label(widthUnit, width, height, position)
    {
}
