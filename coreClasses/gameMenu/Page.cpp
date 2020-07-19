#include "Page.h"
#include <algorithm>

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Map.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

Page::Page(const std::vector<std::shared_ptr<const Label> >& labels,
    const std::shared_ptr<const Page>& parent,
    bool visibleOnParent,
    float height) :
    _labels(labels),
    _bridges{},
    _parent(parent),
    _visibleOnParent(visibleOnParent),
    _height(height),
    _localPosY(0.f),
    _localPressedPosY(0.f),
    _localReleasedPosY(0.f),
    _isPressed(false),
    _pressedScreenPosY(0.f),
    _lastUpdate{},
    _lastSwipeUpdates{},
    _countingUpdates(0)
{}

const std::vector<std::shared_ptr<const Label> >& Page::labels() const{
    return _labels;
}

const std::map<std::shared_ptr<const Label>, std::shared_ptr<Page> >&
Page::bridges() {
    return _bridges;
}

const std::weak_ptr<const Page>& Page::parent() const {
    return _parent;
}

float Page::localPosY() const {
    return _localPosY;
}

bool Page::visibleOnParent() const {
    return _visibleOnParent;
}

std::shared_ptr<Page> Page::child(
        const std::shared_ptr<const Label> &label) {

    if ( _bridges.find(label) != _bridges.end() ) {
        return _bridges.at(label);
    }
    else {
       return nullptr;
    }

}

std::shared_ptr<const Page> Page::child(float x, float y) const{

    std::shared_ptr<const Page> childPage = nullptr;
    if (const std::shared_ptr<const Label> label = matchedLabel(x,y)) {
        if ( _bridges.find(label) != _bridges.end() ) {
            childPage = _bridges.at(label);
        }
    }
    return childPage;
}

float Page::height() const {
    return _height;
}

void Page::addBridge(const std::shared_ptr<const Label> label,
                     const std::shared_ptr<Page> page) {
    if (std::find(_labels.begin(), _labels.end(), label) == _labels.end()) {
        std::cout << "Trying to associate a label that does not exist in the" <<
            "current page ... the operation is skipped ..." << std::endl;
    } else {
        _bridges[label] = page;
    }
}

std::shared_ptr<const Label> Page::matchedLabel(float x, float y) const
{
    for (const std::shared_ptr<const Label>& label : _labels) {
        if (x > (label->position().x - label->width()/2.f) &&
                x < (label->position().x + label->width()/2.f) &&
                y > (label->position().y - label->height()/2.f) &&
                y < (label->position().y + label->height()/2.f)) {
            return label;
        }
    }
    return nullptr;
}

void Page::updateTimeSlide() {
}

void Page::update(bool isPressed, float screenPosY) {


    constexpr float cancelingThresholdDuration = 1.f;
    const auto now = JumperBallTypesMethods::getTimePointMSNow();

    //We reinit the keyframes if the page are not updated for 1 seconde
    if (_countingUpdates > 0 && JumperBallTypesMethods::getFloatFromDurationMS(
            now - _lastUpdate ) > cancelingThresholdDuration) {
        _countingUpdates = 0;
        _isPressed = false;
    }

    _lastUpdate = now;

    //Press cases
    if (isPressed && !_isPressed) {
        _countingUpdates = 0;
        _pressedScreenPosY = screenPosY;
        _localPressedPosY = _localPosY;
        _isPressed = true;
    }

    if (_isPressed) {
        constexpr float thresholdDeltaT = 0.05f; //50 ms
        const bool thresholdIsCrossed =
                (JumperBallTypesMethods::getFloatFromDurationMS(
                     now - _lastSwipeUpdates.at(0).first ) > thresholdDeltaT);
        if (_countingUpdates < 2 ) {
            if ( _countingUpdates == 0 || thresholdIsCrossed ) {
                _lastSwipeUpdates.at(_countingUpdates).first = now;
                _lastSwipeUpdates.at(_countingUpdates).second = screenPosY;
                ++_countingUpdates;
            }
        } else if ( thresholdIsCrossed  ){
            std::cout << "updating" << std::endl;
            _lastSwipeUpdates.at(0) = std::move(_lastSwipeUpdates.at(1));
            _lastSwipeUpdates.at(1).first = now;
            _lastSwipeUpdates.at(1).second = screenPosY;
        }
        _localPosY = _localPressedPosY + (screenPosY - _pressedScreenPosY);
    }

    //Release cases
    if (!isPressed && _isPressed) {
        if (_countingUpdates == 2 ) {
            float deltaT = JumperBallTypesMethods::getFloatFromDurationMS(
                _lastSwipeUpdates.at(1).first -_lastSwipeUpdates.at(0).first );

            // the velocity is the position derivative (pourcentagePage / ms)
            _releaseVelocity =  (_lastSwipeUpdates.at(1).second -
                _lastSwipeUpdates.at(0).second) / std::move(deltaT);
            _localReleasedPosY = _localPosY;
            _isPressed = false;
        }
    }
    if (!_isPressed) {
        if (_countingUpdates == 2) {
            const float t = JumperBallTypesMethods::getFloatFromDurationMS(
                        now - _lastSwipeUpdates.at(1).first);
            float deceleration = decelerationCoefficient * powf(t,2.f)/2.f;

            if (_releaseVelocity > 0.f &&
                    t < -(_releaseVelocity)/(2.f *-decelerationCoefficient/2.f) ) {
                _localPosY = -deceleration +
                        _releaseVelocity * t + _localReleasedPosY ;
            }
            else if (_releaseVelocity < 0.f &&
                    t < -(_releaseVelocity)/(2.f *decelerationCoefficient/2.f) ) {
                _localPosY = deceleration +
                        _releaseVelocity * t + _localReleasedPosY ;
            }

            if (_localPosY < 0.f) _localPosY = 0.f;
            else if (_localPosY > _height - 1.f) _localPosY = _height - 1.f;
        }
    }

}

