/*
 * File: ItemLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#ifndef OBJECTLABEL_H
#define OBJECTLABEL_H
#include "Label.h"


class ItemLabel:public Label {
public:
ItemLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    const Item::CategoryOfItems& categoryOfItem
    );

private:
const Item::CategoryOfItems _categoryOfItems;
};

#endif // OBJECTLABEL_H
