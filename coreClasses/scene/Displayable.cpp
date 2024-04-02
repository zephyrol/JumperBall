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

std::vector<glm::vec2> Displayable::getStaticVec2fValues() const {
    return {};
}

std::vector<glm::vec3> Displayable::getStaticVec3fValues() const {
    return {};
}

std::vector<int> Displayable::getDynamicIntValues() const {
    return {};
}

std::vector<float> Displayable::getDynamicFloatValues() const {
    return {};
}

std::vector<glm::vec2> Displayable::getDynamicVec2fValues() const {
    return {};
}

std::vector<glm::vec3> Displayable::getDynamicVec3fValues() const {
    return {};
}

std::vector<glm::quat> Displayable::getDynamicQuaternionValues() const {
    return {};
}

std::string Displayable::getDynamicGroupHash() const {
    return "";
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
