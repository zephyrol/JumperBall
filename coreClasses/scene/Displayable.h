/*
 * File: Displayable.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18h52
 */
#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H

#include "system/Types.h"
#include "Shape.h"

class Displayable;

using Displayable_sptr = std::shared_ptr<Displayable>;
using CstDisplayable_sptr = std::shared_ptr<const Displayable>;
using vecCstDisplayable_sptr = std::vector<CstDisplayable_sptr>;
using vecDisplayable_sptr = std::vector<Displayable_sptr>;
using CstDisplayable_wptr = std::weak_ptr<const Displayable>;

class Displayable {

public:

    template<typename T> using StaticValues = std::vector<T>;

    template<typename T> using DynamicValues = std::vector<T>;

    using DynamicNames = std::vector<std::string>;

    virtual ~Displayable() = default;

    virtual std::vector<unsigned char> getStaticUnsignedByteValues() const;

    virtual std::vector<short> getStaticShortValues() const;

    virtual StaticValues<float> getStaticFloatValues() const;

    virtual StaticValues<glm::vec2> getStaticVec2fValues() const;

    virtual StaticValues<glm::vec3> getStaticVec3fValues() const;

    virtual DynamicNames getDynamicIntNames() const;

    virtual DynamicValues<int> getDynamicIntValues() const;

    virtual DynamicNames getDynamicFloatNames() const;

    virtual DynamicValues<float> getDynamicFloatValues() const;

    virtual DynamicNames getDynamicVec2fNames() const;

    virtual DynamicValues<glm::vec2> getDynamicVec2fValues() const;

    virtual DynamicNames getDynamicVec3fNames() const;

    virtual DynamicValues<glm::vec3> getDynamicVec3fValues() const;

    virtual DynamicNames getDynamicQuaternionNames() const;

    virtual DynamicValues<glm::quat> getDynamicQuaternionValues() const;

    virtual std::string getDynamicGroupHash() const;

    virtual vecCstShape_sptr getShapes() const;

};

#endif // DISPLAYABLE_H
