/*
 * File:   ObjectState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H
#include <scene/objects/Object.h>


class ObjectState
{
public:
    ObjectState(const Object& object);
    void update();

    bool isGotten() const;
    const JBTypes::timePointMs& timeOfCreation() const;
    const JBTypes::timePointMs& timeOfObtaining() const;
    const Object::CategoryOfObjects& getCategory() const;

private:
    const Object& _object;
    bool _isGotten;
    const JBTypes::timePointMs& _timeOfCreation;
    JBTypes::timePointMs _timeOfObtaining;
    const Object::CategoryOfObjects& _categoryOfObject;

};

#endif // OBJECTSTATE_H
