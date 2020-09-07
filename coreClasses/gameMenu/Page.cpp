#include "Page.h"
#include <math.h>
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

Page::Page(const CstPage_sptr& parent,
           std::map<CstLabel_sptr, Page_sptr> &&bridges,
           float height,
           bool visibleOnParent ):
    _bridges(std::move(bridges)),
    _labels(createLabels()),
    _children(createChildren()),
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

/*const vecCstLabel_sptr& Page::labels() const{
    return _labels;
}*/

/*const std::map<CstLabel_sptr, Page_sptr >& Page::bridges() const{
    return _bridges;
}*/

const std::weak_ptr<const Page>& Page::parent() const {
    return _parent;
}

float Page::localPosY() const {
    return _localPosY;
}

bool Page::visibleOnParent() const {
    return _visibleOnParent;
}

CstPage_sptr Page::child( const CstLabel_sptr& label) const {

    if ( _bridges.find(label) != _bridges.end() ) {
        return _bridges.at(label);
    }
    else {
        return nullptr;
    }

}

Page_sptr Page::child(const CstLabel_sptr &label)
{
    CstPage_sptr cstPage = static_cast<const Page&>(*this).child(label);
    return std::const_pointer_cast<Page>(cstPage);
}

const vecCstLabel_sptr& Page::labels() const
{
    return _labels;
}

CstPage_sptr Page::child(float x, float y) const {

    CstPage_sptr childPage = nullptr;
    if (const CstLabel_sptr label = matchedLabel(x,y)) {
        if ( _bridges.find(label) != _bridges.end() ) {
            childPage = _bridges.at(label);
        }
    }
    return childPage;
}

float Page::height() const {
    return _height;
}

/*void Page::addBridge(const CstLabel_sptr& label, const Page_sptr& page) {
    if (std::find(_labels.begin(), _labels.end(), label) == _labels.end()) {
        std::cout << "Trying to associate a label that does not exist in the" <<
            "current page ... the operation is skipped ..." << std::endl;
    } else {
        _bridges[label] = page;
    }
}*/

CstLabel_sptr Page::matchedLabel(float x, float y) const
{
    for (const CstLabel_sptr& label: _labels) {
        if (x > (label->position().x - label->width()/2.f) &&
                x < (label->position().x + label->width()/2.f) &&
                y > (label->position().y + _localPosY - label->height()/2.f) &&
                y < (label->position().y + _localPosY + label->height()/2.f)) {
            return label;
        }
    }
    return nullptr;
}

void Page::update(bool isPressed, float screenPosY) {

    if (_height - 1.f < EPSILON_F) {
        return;
    }

    constexpr float cancelingThresholdDuration = 1.f;
    const auto now = JBTypesMethods::getTimePointMSNow();

    //We reinit the keyframes if the page are not updated for 1 seconde
    if (_countingUpdates > 0 && JBTypesMethods::getFloatFromDurationMS(
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
                (JBTypesMethods::getFloatFromDurationMS(
                     now - _lastSwipeUpdates.at(0).first ) > thresholdDeltaT);
        if (_countingUpdates < 2 ) {
            if ( _countingUpdates == 0 || thresholdIsCrossed ) {
                _lastSwipeUpdates.at(_countingUpdates).first = now;
                _lastSwipeUpdates.at(_countingUpdates).second = screenPosY;
                ++_countingUpdates;
            }
        } else if ( thresholdIsCrossed  ){
            _lastSwipeUpdates.at(0) = std::move(_lastSwipeUpdates.at(1));
            _lastSwipeUpdates.at(1).first = now;
            _lastSwipeUpdates.at(1).second = screenPosY;
        }
        _localPosY = _localPressedPosY + (screenPosY - _pressedScreenPosY);
    }

    //Release cases
    if (!isPressed && _isPressed) {
        if (_countingUpdates == 2 ) {
            float deltaT = JBTypesMethods::getFloatFromDurationMS(
                _lastSwipeUpdates.at(1).first -_lastSwipeUpdates.at(0).first );

            // the velocity is the position derivative (pourcentagePage / ms)
            _releaseVelocity =  (_lastSwipeUpdates.at(1).second -
                _lastSwipeUpdates.at(0).second) / std::move(deltaT);
            _localReleasedPosY = _localPosY;
            _isPressed = false;
        }
    }
    if (!_isPressed &&_countingUpdates == 2) {
        const float t = JBTypesMethods::getFloatFromDurationMS(
                    now - _lastSwipeUpdates.at(1).first);
        const float deceleration =
                decelerationCoefficient * powf(t,2.f)/2.f;

        if (_releaseVelocity > 0.f && t < -(_releaseVelocity)/
                (2.f *-decelerationCoefficient/2.f) ) {
            _localPosY = -deceleration +
                    _releaseVelocity * t + _localReleasedPosY ;
        }
        else if (_releaseVelocity < 0.f && t < -(_releaseVelocity)/
                 (2.f *decelerationCoefficient/2.f) ) {
            _localPosY = deceleration +
                    _releaseVelocity * t + _localReleasedPosY ;
        }

        if (_localPosY < 0.f) _localPosY = 0.f;
        else if (_localPosY > _height - 1.f) _localPosY = _height - 1.f;
    }

}

std::vector<CstLabel_sptr> Page::createLabels() const {
    std::vector<CstLabel_sptr> labels;
    for (const std::pair<CstLabel_sptr, CstPage_sptr>& bridge: _bridges) {
        labels.push_back(bridge.first);
    }
    return labels;
}

std::vector<Page_sptr> Page::createChildren() const {
    std::vector<Page_sptr> children;
    for (const std::pair<CstLabel_sptr, Page_sptr>& bridge: _bridges) {
        children.push_back(bridge.second);
    }
    return children;
}
