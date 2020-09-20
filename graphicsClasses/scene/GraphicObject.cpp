/*
 * File:   GraphicObject.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "GraphicObject.h"

GraphicObject::GraphicObject(const Object& object):
   _object(object),
   _isGotten(object.isGotten()),
   _timeOfCreation(object.timeOfCreation()),
   _timeOfObtaining(object.timeOfObtaining()),
   _categoryOfObject(object.getCategory())
{

}

void GraphicObject::update()
{
   _isGotten = _object.isGotten();
   _timeOfCreation = _object.timeOfCreation();
   _timeOfObtaining = _object.timeOfObtaining();
   _categoryOfObject = _object.getCategory();
}

bool GraphicObject::isGotten() const
{
    return _isGotten;
}

const JBTypes::timePointMs &GraphicObject::timeOfCreation() const
{
    return _timeOfCreation;
}

const JBTypes::timePointMs &GraphicObject::timeOfObtaining() const
{
    return _timeOfObtaining;
}

const Object::CategoryOfObjects &GraphicObject::getCategory() const
{
    return _categoryOfObject;
}
