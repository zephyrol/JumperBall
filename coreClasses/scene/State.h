/*
 * File: State.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18h52
 */
#ifndef STATE_H
#define STATE_H
#include "system/Types.h"

class State;
using State_sptr = std::shared_ptr <State>;
using CstState_sptr = std::shared_ptr <const State>;
using vecCstState_sptr = std::vector <CstState_sptr>;
using vecState_sptr = std::vector <State_sptr>;
using State_uptr = std::unique_ptr <State>;

class State {

public:

template<typename T> using StaticValues = std::vector <T>;
template<typename T> using DynamicValues = std::map <std::string, T>;
template<typename T> using ExposableValues = std::map <std::string, T>;

State(ExposableValues <float>&& exposableFloats = {},
      ExposableValues <JBTypes::vec3f>&& exposableVec3fs = {});

enum class GlobalState { United, Separate, Dead };
virtual GlobalState update() = 0;
virtual ~State() = default;

virtual StaticValues <float> getStaticFloatValues() const;
virtual StaticValues <JBTypes::vec2f> getStaticVec2fValues() const;
virtual StaticValues <JBTypes::vec3f> getStaticVec3fValues() const;

virtual DynamicValues <float> getDynamicFloats() const;
virtual DynamicValues <JBTypes::vec2f> getDynamicVec2fs() const;
virtual DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const;
virtual DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const;

float getExposableFloat(const std::string& name) const;
const JBTypes::vec3f& getExposableVec3f(const std::string& name) const;

protected:

void setExposableFloat(const std::string& name, float floatValue);
void setExposableVec3f(const std::string& name, const JBTypes::vec3f& vector3f);

private:
ExposableValues <float> _exposableFloats;
ExposableValues <JBTypes::vec3f> _exposableVec3fs;


};

#endif // STATE_H
