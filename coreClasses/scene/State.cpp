/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "State.h"

State::State(ExposableValues <float>&& exposableFloats, ExposableValues <JBTypes::vec3f>&& exposableVec3fs):
    _exposableFloats(std::move(exposableFloats)),
    _exposableVec3fs(std::move(exposableVec3fs)) {
}


std::vector <float> State::getStaticFloatValues() const {
    return {};
}

std::vector <JBTypes::vec2f> State::getStaticVec2fValues() const {
    return {};
}

std::vector <JBTypes::vec3f> State::getStaticVec3fValues() const {
    return {};
}

std::map <std::string, float> State::getDynamicFloats() const {
    return {};
}

std::map <std::string, JBTypes::vec2f> State::getDynamicVec2fs() const {
    return {};
}

std::map <std::string, JBTypes::vec3f> State::getDynamicVec3fs() const {
    return {};
}

std::map <std::string, JBTypes::Quaternion> State::getDynamicQuaternions() const {
    return {};
}

float State::getExposableFloat (const std::string& name) const {
    return _exposableFloats.at(name);
}

const JBTypes::vec3f& State::getExposableVec3f (const std::string& name) const {
    return _exposableVec3fs.at(name);
}

void State::setExposableFloat (const std::string& name, float floatValue) {
    _exposableFloats.at(name) = floatValue;
}

void State::setExposableVec3f (const std::string& name, const JBTypes::vec3f& vector3f) {
    _exposableVec3fs.at(name) = vector3f;
}
