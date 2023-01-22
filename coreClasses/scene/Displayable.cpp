/*
 * File: Displayable.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Displayable.h"

Displayable::Displayable() = default;


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

bool Displayable::globalStateMayChange() const {
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

Displayable::DynamicNames Displayable::getDynamicNames() const {
    auto dynamicNames = getDynamicQuaternionNames();
    auto dynamicVec3fNames = getDynamicVec3fNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(dynamicVec3fNames.begin()),
        std::make_move_iterator(dynamicVec3fNames.end())
    );
    auto dynamicVec2fNames = getDynamicVec2fNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(dynamicVec2fNames.begin()),
        std::make_move_iterator(dynamicVec2fNames.end())
    );
    auto dynamicFloatNames = getDynamicFloatNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(dynamicFloatNames.begin()),
        std::make_move_iterator(dynamicFloatNames.end())
    );
    auto dynamicIntNames = getDynamicIntNames();
    dynamicNames.insert(
        dynamicNames.end(),
        std::make_move_iterator(dynamicIntNames.begin()),
        std::make_move_iterator(dynamicIntNames.end())
    );
    return dynamicNames;
}
