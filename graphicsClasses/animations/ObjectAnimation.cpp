/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ObjectAnimation.cpp
 * Author: Morgenthaler S
 * 
 * Created on 3 avril 2020, 11:36
 */

#include "ObjectAnimation.h"
#include <math.h>

ObjectAnimation::ObjectAnimation(const Object& object,
                                 const glm::vec3& blockPosition,
                                 const JBTypes::Dir& dir):
_object(object),
_referenceTimePointCreation(JBTypesMethods::getTimePointMSNow()),
_direction(dir),
_translationToBlock(glm::translate(blockPosition)),
_initialRotation(Utility::rotationUpToDir(_direction)),
_movingRotation(1.f),
_movingScale(1.f)
{
}

glm::mat4 ObjectAnimation::model() const {
    return  _translationToBlock * translationCenter * _initialRotation * translationOnBlock * _movingRotation;
}

glm::mat4 ObjectAnimation::scaleRotation() const {
    return _initialRotation * _movingRotation;
}

glm::mat4 ObjectAnimation::translation() const {
    return _translationToBlock * translationOnBlock;
}

void ObjectAnimation::updateTrans() {
    const float seconds = JBTypesMethods::getTimeSecondsSinceTimePoint(
      _referenceTimePointCreation);
    
    constexpr float speedFactor = 5.f;
    _movingRotation = glm::rotate(speedFactor* seconds,glm::vec3(0.f,1.f,0.f));

}

const Object &ObjectAnimation::object() const {
    return _object;
}


const glm::mat4 ObjectAnimation::translationOnBlock = 
                          glm::translate(glm::vec3(0.f,0.75f,0.f));

const glm::mat4 ObjectAnimation::translationCenter = 
                          glm::translate(glm::vec3(0.5f,0.5f,0.5f));

const glm::mat4 ObjectAnimation::inversedTranslationCenter = 
    glm::inverse(ObjectAnimation::translationCenter);
