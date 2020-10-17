/*
 * File:   ObjectState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "ObjectState.h"

ObjectState::ObjectState(const Object& object):
   _object(object),
   _isGotten(object.isGotten()),
   _timeOfCreation(object.timeOfCreation()),
   _timeOfObtaining(object.timeOfObtaining()),
   _categoryOfObject(object.getCategory())
{

}

void ObjectState::update()
{
   _isGotten = _object.isGotten();
   _timeOfObtaining = _object.timeOfObtaining();
}

bool ObjectState::isGotten() const
{
    return _isGotten;
}

const JBTypes::timePointMs &ObjectState::timeOfCreation() const
{
    return _timeOfCreation;
}

const JBTypes::timePointMs &ObjectState::timeOfObtaining() const
{
    return _timeOfObtaining;
}

const Object::CategoryOfObjects &ObjectState::getCategory() const
{
    return _categoryOfObject;
}
