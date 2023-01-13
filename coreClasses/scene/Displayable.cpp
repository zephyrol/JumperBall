/*
 * File: Displayable.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Displayable.h"

Displayable::Displayable() = default;

std::vector<short> Displayable::getStaticIntValues() const {
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

std::unordered_map<std::string, int> Displayable::getDynamicInts() const {
    return {};
}

std::unordered_map<std::string, float> Displayable::getDynamicFloats() const {
    return {};
}

std::unordered_map<std::string, JBTypes::vec2f> Displayable::getDynamicVec2fs() const {
    return {};
}

std::unordered_map<std::string, JBTypes::vec3f> Displayable::getDynamicVec3fs() const {
    return {};
}

std::unordered_map<std::string, JBTypes::Quaternion> Displayable::getDynamicQuaternions() const {
    return {};
}

bool Displayable::globalStateMayChange() const {
    return false;
}
