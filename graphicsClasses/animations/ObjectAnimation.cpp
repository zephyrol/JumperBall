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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> 
#include <math.h>

ObjectAnimation::ObjectAnimation(const Object& object,
                                 const glm::vec3& blockPosition,
                                 const JumperBallTypes::Direction& dir):
_object(object),
_referenceTimePointCreation(JumperBallTypesMethods::getTimePointMSNow()),
_direction(dir),
_translationToBlock(glm::translate(blockPosition)),
_initialRotation(Utility::rotationUpToDir(dir))
{
}

glm::mat4 ObjectAnimation::model() const {
    return  _translationToBlock *
             _initialRotation * translationOnBlock * inversedTranslationCenter
              * _movingRotation * translationCenter;
}

glm::mat4 ObjectAnimation::scaleRotation() const {
    return _initialRotation * _movingRotation;
}

glm::mat4 ObjectAnimation::translation() const {
    return _translationToBlock * translationOnBlock;
}



void ObjectAnimation::updateTrans() {
    float seconds = JumperBallTypesMethods::getTimeSecondsSinceTimePoint(
      _referenceTimePointCreation);
    
    constexpr float speedFactor = 5.f;
    _movingRotation = glm::rotate(speedFactor* seconds,glm::vec3(0.f,1.f,0.f));

}

const glm::mat4 ObjectAnimation::translationOnBlock = 
                          glm::translate(glm::vec3(0.f,1.f,0.f));

const glm::mat4 ObjectAnimation::translationCenter = 
                          glm::translate(glm::vec3(-0.5f,-0.3f,-0.5f));

const glm::mat4 ObjectAnimation::inversedTranslationCenter = 
glm::inverse(ObjectAnimation::translationCenter);
