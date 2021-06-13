/*
 * File: ObjectState.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "ObjectState.h"

ObjectState::ObjectState() {
}

std::vector <float> ObjectState::getStaticFloatValues() const {
    return {};
}

std::vector <JBTypes::vec2f> ObjectState::getStaticVec2fValues() const {
    return {};
}

std::vector <JBTypes::vec3f> ObjectState::getStaticVec3fValues() const {
    return {};
}

std::map <std::string, float> ObjectState::getDynamicFloats() const {
    return {};
}

std::map <std::string, JBTypes::vec2f> ObjectState::getDynamicVec2fs() const {
    return {};
}

std::map <std::string, JBTypes::vec3f> ObjectState::getDynamicVec3fs() const {
    return {};
}

std::map <std::string, JBTypes::Quaternion> ObjectState::getDynamicQuaternions() const {
    return {};
}
