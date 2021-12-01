/*
 * File: SceneElement.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "SceneElement.h"

SceneElement::SceneElement() {
}

std::vector <float> SceneElement::getStaticFloatValues() const {
    return {};
}

std::vector <JBTypes::vec2f> SceneElement::getStaticVec2fValues() const {
    return {};
}

std::vector <JBTypes::vec3f> SceneElement::getStaticVec3fValues() const {
    return {};
}

std::unordered_map <std::string, float> SceneElement::getDynamicFloats() const {
    return {};
}

std::unordered_map <std::string, JBTypes::vec2f> SceneElement::getDynamicVec2fs() const {
    return {};
}

std::unordered_map <std::string, JBTypes::vec3f> SceneElement::getDynamicVec3fs() const {
    return {};
}

std::unordered_map <std::string, JBTypes::Quaternion> SceneElement::getDynamicQuaternions() const {
    return {};
}
