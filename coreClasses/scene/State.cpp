/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "State.h"


State::State():
    _globalState(State::GlobalState::Alive) {
}

std::vector <unsigned char> State::getStaticBytesValues() const {
    return {};
}

std::vector <unsigned int> State::getStaticUIntValues() const {
    return {};
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

std::map <std::string, unsigned char> State::getDynamicUChars() const {
    return {};
}

std::map <std::string, unsigned int> State::getDynamicUInts() const {
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

const State::GlobalState& State::getGlobalState() const {
    return _globalState;
}
