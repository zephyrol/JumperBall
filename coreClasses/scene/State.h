/*
 * File: State.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18h52
 */
#ifndef STATE_H
#define STATE_H
#include "system/Types.h"

class ObjectState;
using State_sptr = std::shared_ptr <ObjectState>;
using CstState_sptr = std::shared_ptr <const ObjectState>;
using vecCstState_sptr = std::vector <CstState_sptr>;
using vecState_sptr = std::vector <State_sptr>;
using State_uptr = std::unique_ptr <ObjectState>;

class ObjectState {

public:

template<typename T> using StaticValues = std::vector <T>;
template<typename T> using DynamicValues = std::map <std::string, T>;

ObjectState();

enum class GlobalState { United, Separate, Dead };
virtual GlobalState update() = 0;
virtual ~ObjectState() = default;

virtual StaticValues <float> getStaticFloatValues() const;
virtual StaticValues <JBTypes::vec2f> getStaticVec2fValues() const;
virtual StaticValues <JBTypes::vec3f> getStaticVec3fValues() const;

virtual DynamicValues <float> getDynamicFloats() const;
virtual DynamicValues <JBTypes::vec2f> getDynamicVec2fs() const;
virtual DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const;
virtual DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const;
};

#endif // STATE_H
