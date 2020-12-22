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

ObjectAnimation::ObjectAnimation(const ObjectState& object,
                                 const glm::vec3& blockPosition,
                                 const JBTypes::Dir& dir):
_object(object),
_direction(dir),
_translationToBlock(glm::translate(blockPosition)),
_initialRotation(Utility::rotationUpToDir(_direction)),
_tToBlockTCenterInitRotTOnBlock(_translationToBlock * translationCenter *
        _initialRotation * translationOnBlock),
_tToBlockTOnBlock(_translationToBlock * translationOnBlock),
_movingRotation(1.f),
_movingScale(1.f),
_movingTranslation(1.f)
{
}

void ObjectAnimation::updateTrans() {
    if (_object.isGotten()) {
        transAfterObtaining();
    } else {
        transBeforeObtaining();
    }

    _model = _tToBlockTCenterInitRotTOnBlock * _movingTranslation * 
        _movingRotation * _movingScale;
    _scaleRotation = _initialRotation * _movingRotation;
    _translation = _movingTranslation * _tToBlockTOnBlock;
}

void ObjectAnimation::transBeforeObtaining() {
    const float seconds = JBTypesMethods::getTimeSecondsSinceTimePoint(
      _object.timeOfCreation());

    constexpr float speedFactor = 5.f;
    _movingRotation = glm::rotate(speedFactor* seconds,rotationAxis);
}

void ObjectAnimation::transAfterObtaining() {
    constexpr float speedPow = 5.f;
    constexpr float thresholdSecondStep = 1.f;
    constexpr float thresholdThirdStep = 1.5f;
    constexpr float durationSecondStep =
        thresholdThirdStep - thresholdSecondStep;
    constexpr float durationThirdStep = 0.2f;

    const float seconds = JBTypesMethods::getTimeSecondsSinceTimePoint(
        _object.timeOfObtaining());

    float scaleFactor;
    float translateFactor;
    if (seconds < thresholdSecondStep) {
        translateFactor = seconds/thresholdSecondStep;
        scaleFactor = 1.f;
    } else if (seconds < thresholdThirdStep) {
        translateFactor = 1.f;
        scaleFactor = 1.f +
                ((seconds - thresholdSecondStep) / durationSecondStep);
    } else if (seconds < thresholdThirdStep + durationThirdStep){
        translateFactor = 1.f;
        scaleFactor = 2.f *
                (1.f - ((seconds - thresholdThirdStep) / durationThirdStep));
    } else {
        translateFactor = 1.f;
        scaleFactor = 0.f;
    }

    _movingScale = glm::scale(scaleFactor * glm::vec3(1.f,1.f,1.f));
    _movingRotation = glm::rotate(powf(seconds+1.f,speedPow), rotationAxis);
    _movingTranslation = glm::translate( translateFactor * rotationAxis);
}

const ObjectState &ObjectAnimation::object() const {
    return _object;
}

const glm::mat4 ObjectAnimation::translationOnBlock = 
                          glm::translate(glm::vec3(0.f,0.75f,0.f));

const glm::mat4 ObjectAnimation::translationCenter = 
                          glm::translate(glm::vec3(0.5f,0.5f,0.5f));

const glm::mat4 ObjectAnimation::inversedTranslationCenter = 
    glm::inverse(ObjectAnimation::translationCenter);

const glm::vec3 ObjectAnimation::rotationAxis = glm::vec3(0.f,1.f,0.f);
