/*
 * File: ObjectState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H
#include <scene/objects/Object.h>
#include <scene/State.h>


class ObjectState:public State {
public:
ObjectState(const Object& object);

bool isGotten() const;
const JBTypes::timePointMs& timeOfObtaining() const;
std::vector <float> getStaticFloatValues() const override;
std::vector <JBTypes::vec3f> getStaticVec3fValues() const override;
std::map <std::string, float> getDynamicFloats() const override;
void update() override;

private:
const Object& _object;
bool _isGotten;
JBTypes::timePointMs _timeOfObtaining;

};

#endif // OBJECTSTATE_H
