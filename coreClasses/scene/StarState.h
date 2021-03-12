/*
 * File: StarState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef STARSTATE_H
#define STARSTATE_H
#include "State.h"
#include "Star.h"


class StarState:public State {
public:
StarState(const Star& star);

void update() override;
const JBTypes::vec3f& lightDirection() const;
std::map <std::string, float> getDynamicFloats() const override;
std::map <std::string, JBTypes::vec3f> getDynamicVec3fs() const override;

private:
const Star& _star;
float _timeSinceCreation;
JBTypes::vec3f _lightDirection;
};


#endif // STARSTATE_H
