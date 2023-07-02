/*
 * File: Displayable.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Displayable.h"


std::vector<unsigned char> Displayable::getStaticUnsignedByteValues() const {
    return {};
}

std::vector<short> Displayable::getStaticShortValues() const {
    return {};
}

std::vector<float> Displayable::getStaticFloatValues() const {
    return {};
}

std::vector<JBTypes::vec2f> Displayable::getStaticVec2fValues() const {
    return {};
}

std::vector<JBTypes::vec3f> Displayable::getStaticVec3fValues() const {
    return {};
}

std::vector<int> Displayable::getDynamicIntValues() const {
    return {};
}

std::vector<float> Displayable::getDynamicFloatValues() const {
    return {};
}

std::vector<JBTypes::vec2f> Displayable::getDynamicVec2fValues() const {
    return {};
}

std::vector<JBTypes::vec3f> Displayable::getDynamicVec3fValues() const {
    return {};
}

std::vector<JBTypes::Quaternion> Displayable::getDynamicQuaternionValues() const {
    return {};
}

std::string Displayable::getDynamicGroupHash() const {
    return "";
}

bool Displayable::dynamicsMayChange() const {
    return false;
}

Displayable::DynamicNames Displayable::getDynamicIntNames() const {
    return {};
}

Displayable::DynamicNames Displayable::getDynamicFloatNames() const {
    return {};
}

Displayable::DynamicNames Displayable::getDynamicVec2fNames() const {
    return {};
}

Displayable::DynamicNames Displayable::getDynamicVec3fNames() const {
    return {};
}

Displayable::DynamicNames Displayable::getDynamicQuaternionNames() const {
    return {};
}

vecCstShape_sptr Displayable::getShapes() const {
    return {};
}
