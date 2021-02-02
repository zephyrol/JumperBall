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
State();
virtual void update() = 0;
virtual ~State() = default;

virtual std::map <std::string, unsigned char> getStaticBytesValues() const;
virtual std::map <std::string, unsigned int> getStaticUIntValues() const;
virtual std::map <std::string, float> getStaticFloatValues() const;
virtual std::map <std::string, JBTypes::vec2f> getStaticVec2fValues() const;
virtual std::map <std::string, JBTypes::vec3f> getStaticVec3fValues() const;

virtual std::vector <unsigned char> getDynamicBytes() const;
virtual std::vector <unsigned int> getDynamicUInt() const;
virtual std::vector <float> getDynamicFloat() const;
virtual std::vector <JBTypes::vec2f> getDynamicVec2f() const;
virtual std::vector <JBTypes::vec3f> getDynamicVec3f() const;
};

#endif // STATE_H
