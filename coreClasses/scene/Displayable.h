/*
 * File: Displayable.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18h52
 */
#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H
#include "system/Types.h"

class Displayable;
using Displayable_sptr = std::shared_ptr <Displayable>;
using CstDisplayable_sptr = std::shared_ptr <const Displayable>;
using vecCstDisplayable_sptr = std::vector <CstDisplayable_sptr>;
using vecDisplayable_sptr = std::vector <Displayable_sptr>;
using Displayable_uptr = std::unique_ptr <Displayable>;

class Displayable {

public:

template<typename T> using StaticValues = std::vector <T>;
template<typename T> using DynamicValues = std::unordered_map <std::string, T>;
enum class GlobalState { United, Separate, Dead };

Displayable();

virtual ~Displayable() = default;

virtual StaticValues <float> getStaticFloatValues() const;
virtual StaticValues <JBTypes::vec2f> getStaticVec2fValues() const;
virtual StaticValues <JBTypes::vec3f> getStaticVec3fValues() const;

virtual DynamicValues <float> getDynamicFloats() const;
virtual DynamicValues <JBTypes::vec2f> getDynamicVec2fs() const;
virtual DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const;
virtual DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const;

virtual GlobalState getGlobalState() const = 0;
virtual bool globalStateMayChange() const;

};

#endif // DISPLAYABLE_ELEMENT_H
