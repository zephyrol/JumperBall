/*
 * File:   SpecialAnimation.cpp
 * Author: Morgenthaler S
 *
 * Created on 18 decembre 2020, 20h15
 */
#include "SpecialAnimation.h"

SpecialAnimation::SpecialAnimation(const SpecialState &special):
    _special(special),
    _initialTranslation(initInitialTranslation()),
    _initialRotation(initInitialRotation()),
    _initialTranslationRotation(_initialTranslation * _initialRotation),
    _movingRotation(glm::mat4(1.f)),
    _movingTranslation(glm::mat4(1.f)),
    _movingScale(glm::mat4(1.f))
{
}

void SpecialAnimation::transTeleporter() {
    if (_special.isActivated()) {
        const float seconds = JBTypesMethods::getTimeSecondsSinceTimePoint(
            _special.creationTime());

        constexpr float speedFactor = 6.f;
        _movingRotation = glm::rotate(speedFactor * seconds, rotationAxis);
    } else {
        _movingRotation = glm::mat4(1.f);
    }
}

void SpecialAnimation::transSwitch() {
    if (_special.isActivated()) {
        _movingScale = glm::scale(glm::vec3(1.f,0.1f,1.f));
    } else {
        _movingScale = glm::scale(glm::vec3(1.f,1.f,1.f));
    }
}

void SpecialAnimation::updateTrans()
{
    if (_special.category() == Map::SpecialTypes::Teleporter) {
        transTeleporter();
    } else {
        transSwitch();
    }
    _model = _movingTranslation * _initialTranslationRotation *  
        _movingRotation * _movingScale;
    _scaleRotation = _initialRotation * _movingRotation;
    _translation = _movingTranslation * _initialTranslation;
}

glm::mat4 SpecialAnimation::initInitialTranslation() const {
    const JBTypes::vec3f& position = _special.position3D();
    return glm::translate(glm::vec3(position.x, position.y, position.z));
}

glm::mat4 SpecialAnimation::initInitialRotation() const
{
    const JBTypes::Dir currentDir = JBTypesMethods::integerAsDirection(
        static_cast<unsigned int>(_special.direction())
    );
    return Utility::rotationUpToDir(currentDir);
}

const glm::vec3 SpecialAnimation::rotationAxis = glm::vec3(0.f,1.f,0.f);
