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

using Displayable_sptr = std::shared_ptr<Displayable>;
using CstDisplayable_sptr = std::shared_ptr<const Displayable>;
using vecCstDisplayable_sptr = std::vector<CstDisplayable_sptr>;
using vecDisplayable_sptr = std::vector<Displayable_sptr>;
using Displayable_uptr = std::unique_ptr<Displayable>;

class Displayable {

public:

    template<typename T> using StaticValues = std::vector<T>;

    template<typename T> using DynamicValues = std::vector<T>;

    using DynamicNames = std::vector<std::string>;

    enum class GlobalState {
        United, Separate, Dead
    };

    Displayable();

    virtual ~Displayable() = default;

    virtual std::vector<unsigned char> getStaticUnsignedByteValues() const;

    virtual std::vector<short> getStaticShortValues() const;

    virtual StaticValues<float> getStaticFloatValues() const;

    virtual StaticValues<JBTypes::vec2f> getStaticVec2fValues() const;

    virtual StaticValues<JBTypes::vec3f> getStaticVec3fValues() const;

    DynamicNames getDynamicNames() const;

    virtual DynamicNames getDynamicIntNames() const;
    virtual DynamicValues<int> getDynamicIntValues() const;

    virtual DynamicNames getDynamicFloatNames() const;
    virtual DynamicValues<float> getDynamicFloatValues() const;

    virtual DynamicNames getDynamicVec2fNames() const;
    virtual DynamicValues<JBTypes::vec2f> getDynamicVec2fValues() const;

    virtual DynamicNames getDynamicVec3fNames() const;
    virtual DynamicValues<JBTypes::vec3f> getDynamicVec3fValues() const;

    virtual DynamicNames getDynamicQuaternionNames() const;
    virtual DynamicValues<JBTypes::Quaternion> getDynamicQuaternionValues() const;

    virtual GlobalState getGlobalState() const = 0;

    virtual bool globalStateMayChange() const;

};

#endif // DISPLAYABLE_ELEMENT_H
