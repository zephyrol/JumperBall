/*
 * File: SceneElement.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18h52
 */
#ifndef SCENE_ELEMENT_H 
#define SCENE_ELEMENT_H 
#include "system/Types.h"

class SceneElement;
using SceneElement_sptr = std::shared_ptr <SceneElement>;
using CstSceneElement_sptr = std::shared_ptr <const SceneElement>;
using vecCstSceneElement_sptr = std::vector <CstSceneElement_sptr>;
using vecObjectState_sptr = std::vector <SceneElement_sptr>;
using SceneElement_uptr = std::unique_ptr <SceneElement>;

class SceneElement {

public:

template<typename T> using StaticValues = std::vector <T>;
template<typename T> using DynamicValues = std::map <std::string, T>;
enum class GlobalState { United, Separate, Dead };

SceneElement();

virtual ~SceneElement() = default;

virtual StaticValues <float> getStaticFloatValues() const;
virtual StaticValues <JBTypes::vec2f> getStaticVec2fValues() const;
virtual StaticValues <JBTypes::vec3f> getStaticVec3fValues() const;

virtual DynamicValues <float> getDynamicFloats() const;
virtual DynamicValues <JBTypes::vec2f> getDynamicVec2fs() const;
virtual DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const;
virtual DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const;

virtual GlobalState getGlobalState() const = 0;

};

#endif // SCENE_ELEMENT_H 
