#include "Page.h"
#include <cmath>
#include <algorithm>


/*
 * File: Map.h
 * Author: Morgenthaler S
 *
 * Created on 28 avril 2020, 18:56
 */

Page::Page(
    Player_sptr&& player,
    float height
) :
    _player(std::move(player)),
    _height(height),
    _localPosY(0.f),
    _localPressedPosY(0.f),
    _localReleasedPosY(0.f),
    _isPressed(false),
    _pressedScreenPosY(0.f),
    _lastUpdate{},
    _lastSwipeUpdates{},
    _releaseVelocity(0.f) {
}

Page_wptr Page::parent() {
    return {};
}

float Page::localPosY() const {
    return _localPosY;
}

float Page::height() const {
    return _height;
}

void Page::update(bool isPressed, float screenPosY) {

    if (JBTypesMethods::floatsEqual(_height, 1.f)) {
        return;
    }

    const auto now = JBTypesMethods::getTimePointMSNow();
    _lastUpdate = now;

    // Press cases
    if (isPressed && !_isPressed) {
        _pressedScreenPosY = screenPosY;
        _localPressedPosY = _localPosY;
        _isPressed = true;
        _lastSwipeUpdates.clear();
    }

    if (_isPressed) {
        constexpr float thresholdDeltaT = 0.1f; // 100 ms
        std::list<slideState>::iterator it;
        for (
            it = _lastSwipeUpdates.begin();
            it != _lastSwipeUpdates.end() &&
            JBTypesMethods::getFloatFromDurationMS(now - it->first) < thresholdDeltaT;
            ++it
            ) {
        }

        _lastSwipeUpdates.erase(it, _lastSwipeUpdates.end());
        _lastSwipeUpdates.push_front({now, screenPosY});
        _localPosY = _localPressedPosY + (screenPosY - _pressedScreenPosY);
    }

    // Release cases
    if (!isPressed && _isPressed && !_lastSwipeUpdates.empty()) {
        const slideState &lastSlideState = _lastSwipeUpdates.front();
        const slideState &olderSlideState = _lastSwipeUpdates.back();
        float deltaT = JBTypesMethods::getFloatFromDurationMS(
            lastSlideState.first - olderSlideState.first);

        // the velocity is the position derivative (pourcentagePage / ms)
        _releaseVelocity = (lastSlideState.second - olderSlideState.second) / deltaT;

        _localReleasedPosY = _localPosY;
        _isPressed = false;
    }
    if (!_isPressed && !_lastSwipeUpdates.empty()) {
        const slideState &lastSlideState = _lastSwipeUpdates.front();
        const float t = JBTypesMethods::getFloatFromDurationMS(now - lastSlideState.first);
        const float deceleration = decelerationCoefficient * powf(t, 2.f) / 2.f;

        if (_releaseVelocity > 0.f && t < -(_releaseVelocity) / (2.f * -decelerationCoefficient / 2.f)) {
            _localPosY = -deceleration + _releaseVelocity * t + _localReleasedPosY;
        } else if (
            _releaseVelocity < 0.f &&
            t < -(_releaseVelocity) / (decelerationCoefficient)
            ) {
            _localPosY = deceleration + _releaseVelocity * t + _localReleasedPosY;
        }

        if (_localPosY < 0.f) _localPosY = 0.f;
        else if (_localPosY > _height - 1.f) _localPosY = _height - 1.f;
    }

}

// TODO: Useless ?
SceneElement::GlobalState Page::getGlobalState() const {
    return SceneElement::GlobalState::United;
}

vecCstLabel_sptr Page::labels() const {
    return {};
}

