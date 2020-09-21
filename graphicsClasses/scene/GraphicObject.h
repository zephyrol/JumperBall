/*
 * File:   GraphicObject.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H
#include <objects/Object.h>


class GraphicObject
{
public:
    GraphicObject(const Object& object);
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

#endif // GRAPHICOBJECT_H
