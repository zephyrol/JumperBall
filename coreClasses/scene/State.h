/*
 * File: State.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18h52
 */
#ifndef STATE_H
#define STATE_H
#include "system/Types.h"

class State {

public:

enum class GlobalState { Alive, Zomby, Dead };
State();
virtual void update() = 0;
virtual ~State() = default;

virtual std::vector <float> getStaticFloatValues() const;
virtual std::vector <JBTypes::vec2f> getStaticVec2fValues() const;
virtual std::vector <JBTypes::vec3f> getStaticVec3fValues() const;

virtual std::map <std::string, float> getDynamicFloats() const;
virtual std::map <std::string, JBTypes::vec2f> getDynamicVec2fs() const;
virtual std::map <std::string, JBTypes::vec3f> getDynamicVec3fs() const;
virtual std::map <std::string, JBTypes::Quaternion> getDynamicQuaternions() const;

const GlobalState& getGlobalState() const;

private:
GlobalState _globalState;
};

#endif // STATE_H
