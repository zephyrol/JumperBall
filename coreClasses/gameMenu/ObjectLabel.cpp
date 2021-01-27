
/*
 * File:   ObjectLabel.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 19:43
 */

#include "ObjectLabel.h"

ObjectLabel::ObjectLabel(
    const Label::WidthUnit& widthUnit,
    float width,
    float height,
    const JBTypes::vec2f& position,
    const Object::CategoryOfObjects& categoryOfObject
  ): 
    Label(widthUnit, width, height, position),
    _categoryOfObjects(categoryOfObject)
{

}
